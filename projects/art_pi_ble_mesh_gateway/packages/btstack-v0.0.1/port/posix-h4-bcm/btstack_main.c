/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

#define __BTSTACK_FILE__ "main.c"

// *****************************************************************************
//
// minimal setup for HCI code
//
// *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "btstack_config.h"

#include "btstack_debug.h"
#include "btstack_event.h"
#include "ble/le_device_db_tlv.h"
#include "classic/btstack_link_key_db_tlv.h"
#include "btstack_memory.h"
#include "btstack_run_loop.h"
#include "btstack_run_loop_posix.h"
#include "bluetooth_company_id.h"
#include "hci.h"
#include "hci_dump.h"
#include "btstack_stdin.h"
#include "btstack_tlv_posix.h"

#include "btstack_chipset_bcm.h"
#include "btstack_chipset_bcm_download_firmware.h"

int btstack_main(int argc, const char *argv[]);

#define TLV_DB_PATH_PREFIX "/flash/btstack_"
#define TLV_DB_PATH_POSTFIX ".tlv"
static char tlv_db_path[100];
static const btstack_tlv_t *tlv_impl;
static btstack_tlv_posix_t tlv_context;

static hci_transport_config_uart_t transport_config = {
    HCI_TRANSPORT_CONFIG_UART,
    115200,
    115200, // main baudrate
    1,      // flow control
    NULL,
};
static btstack_uart_config_t uart_config;

static int main_argc;
static const char **main_argv;

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void sigint_handler(int param)
{
    UNUSED(param);

    printf("CTRL-C - SIGINT received, shutting down..\n");
    log_info("sigint_handler: shutting down");

    // reset anyway
    btstack_stdin_reset();

    // power down
    hci_power_control(HCI_POWER_OFF);
    hci_close();
    log_info("Good bye, see you.\n");
    //    exit(0);
}

static int led_state = 0;
void hal_led_toggle(void)
{
    led_state = 1 - led_state;
    printf("LED State %u\n", led_state);
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    bd_addr_t addr;
    if (packet_type != HCI_EVENT_PACKET)
        return;
    switch (hci_event_packet_get_type(packet))
    {
    case BTSTACK_EVENT_STATE:
        if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
            break;
        gap_local_bd_addr(addr);
        printf("BTstack up and running at %s\n", bd_addr_to_str(addr));
        // setup TLV
        //            strcpy(tlv_db_path, TLV_DB_PATH_PREFIX);
        //            strcat(tlv_db_path, bd_addr_to_str(addr));
        //            strcat(tlv_db_path, TLV_DB_PATH_POSTFIX);
        //            tlv_impl = btstack_tlv_posix_init_instance(&tlv_context, tlv_db_path);
        //            btstack_tlv_set_instance(tlv_impl, &tlv_context);
        //#ifdef ENABLE_CLASSIC
        //            hci_set_link_key_db(btstack_link_key_db_tlv_get_instance(tlv_impl, &tlv_context));
        //#endif
        //#ifdef ENABLE_BLE
        //            le_device_db_tlv_configure(tlv_impl, &tlv_context);
        //#endif
        break;
    default:
        break;
    }
}

static void phase2(int status);
void bt_stack_main(void *param)
{

    /// GET STARTED with BTstack ///
    btstack_memory_init();

    // use logger: format HCI_DUMP_PACKETLOGGER, HCI_DUMP_BLUEZ or HCI_DUMP_STDOUT
    //const char * pklg_path = "/tmp/hci_dump.pklg";
    // hci_dump_open(NULL, HCI_DUMP_STDOUT);
    // printf("Packet Log: %s\n", pklg_path);

    strcpy(tlv_db_path, TLV_DB_PATH_PREFIX);
    //   strcat(tlv_db_path, bd_addr_to_str(addr));
    strcat(tlv_db_path, TLV_DB_PATH_POSTFIX);
    tlv_impl = btstack_tlv_posix_init_instance(&tlv_context, tlv_db_path);
    btstack_tlv_set_instance(tlv_impl, &tlv_context);
#ifdef ENABLE_CLASSIC
    hci_set_link_key_db(btstack_link_key_db_tlv_get_instance(tlv_impl, &tlv_context));
#endif
#ifdef ENABLE_BLE
    le_device_db_tlv_configure(tlv_impl, &tlv_context);
#endif
    // setup run loop
    btstack_run_loop_init(btstack_run_loop_posix_get_instance());

    // pick serial port and configure uart block driver
    transport_config.device_name = "/dev/uart3"; // RedBear IoT pHAT breakout board

    // get BCM chipset driver
    const btstack_chipset_t *chipset = btstack_chipset_bcm_instance();
    chipset->init(&transport_config);

    // set chipset name
    btstack_chipset_bcm_set_device_name("/dev/bt_image");

    // setup UART driver
    const btstack_uart_block_t *uart_driver = btstack_uart_block_posix_instance();

    // extract UART config from transport config
    uart_config.baudrate = transport_config.baudrate_init;
    uart_config.flowcontrol = transport_config.flowcontrol;
    uart_config.device_name = transport_config.device_name;
    uart_driver->init(&uart_config);

#define BT_AP6212_PIN GET_PIN(I, 11)
    rt_pin_mode(BT_AP6212_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(BT_AP6212_PIN, PIN_LOW);
    HAL_Delay(1000);
    rt_pin_write(BT_AP6212_PIN, PIN_HIGH);
    HAL_Delay(1000);

    // setup HCI (to be able to use bcm chipset driver)
    // init HCI
    const hci_transport_t *transport = hci_transport_h4_instance(uart_driver);
    hci_init(transport, (void *)&transport_config);
    hci_set_chipset(btstack_chipset_bcm_instance());

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // handle CTRL-c
    signal(SIGINT, sigint_handler);

    // phase #1 download firmware
    printf("Phase 1: Download firmware\n");

    // phase #2 start main app
    btstack_chipset_bcm_download_firmware(uart_driver, transport_config.baudrate_main, &phase2);

    //default mac
    bd_addr_t own_address = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

    //config mac
    own_address[0] = 0x41; //A
    own_address[1] = 0x52; //R
    own_address[2] = 0x54; //T
    own_address[3] = *(rt_uint8_t *)(UID_BASE + 4);
    own_address[4] = *(rt_uint8_t *)(UID_BASE + 2);
    own_address[5] = *(rt_uint8_t *)(UID_BASE + 0);

    hci_set_bd_addr(own_address);
    // go
    btstack_run_loop_execute();
}

void bt_stack_port_main(void)
{
    static rt_thread_t tid1 = RT_NULL;

    tid1 = rt_thread_create("bt_stack",
                            bt_stack_main, RT_NULL,
                            4096,
                            5, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
}
MSH_CMD_EXPORT(bt_stack_port_main, btstack start);

static void phase2(int status)
{

    if (status)
    {
        printf("Download firmware failed\n");
        return;
    }

    printf("Phase 2: Main app\n");

    // setup app
    btstack_main(main_argc, main_argv);
}
