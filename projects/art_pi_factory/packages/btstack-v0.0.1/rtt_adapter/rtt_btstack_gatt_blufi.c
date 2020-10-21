/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-14     super_mcu    first version
 */

#define BTSTACK_FILE__ "gatt_blufi_config.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtt_btstack_gatt_blufi.h"
#include "btstack.h"
#include "rtthread.h"

#include "wifi.h"

#define HEARTBEAT_PERIOD_MS 1000

static int le_notification_enabled = 0;
static btstack_timer_source_t heartbeat;
static btstack_packet_callback_registration_t hci_event_callback_registration;
static hci_con_handle_t con_handle;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static uint16_t att_read_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
static int att_write_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
static void heartbeat_handler(struct btstack_timer_source *ts);
//static void beat(void);

const uint8_t adv_data[] = {
    // Flags general discoverable, BR/EDR not supported
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x05,
    // Name
    0x0b, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'A', 'R', 'T', '-', 'P', 'i', ' ', ' ', ' ', ' ',
    // Incomplete List of 16-bit Service Class UUIDs -- FF10 - only valid for testing!
};
const uint8_t adv_data_len = sizeof(adv_data);

static void le_counter_setup(void)
{

    l2cap_init();

    // setup le device db
    le_device_db_init();

    // setup SM: Display only
    sm_init();

    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t *)adv_data);
    gap_advertisements_enable(1);

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);

    // set one-shot timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);

    // beat once
    //   beat();
}
/* LISTING_END */

/*
 * @section Heartbeat Handler
 *
 * @text The heartbeat handler updates the value of the single Characteristic provided in this example,
 * and request a ATT_EVENT_CAN_SEND_NOW to send a notification if enabled see Listing heartbeat.
 */

/* LISTING_START(heartbeat): Hearbeat Handler */
//static int counter = 0;
static char counter_string[1024];
static int counter_string_len;
static uint32_t send_buffer_index = 0;
static uint8_t send_buffer_complete = 1;
#define GATT_SEND_MAX_SIZE 20

#define BLUFI_RECV_BUFF_SIZE 200
struct basl_blufi_recv_data
{
    uint8_t buf[BLUFI_RECV_BUFF_SIZE];
    uint16_t alloc_len;
    uint8_t recv_offset;
};

static struct basl_blufi_recv_data blufi_data;
uint8_t bsal_blufi_push_data(struct basl_blufi_recv_data *blufi_data, uint8_t length, uint8_t *data)
{
    if (blufi_data->recv_offset + length > blufi_data->alloc_len)
    {
        //error
        return 1;
    }
    if (data[0] == '{')
    {
        //first entry
        blufi_data->recv_offset = 0;
        memset(blufi_data->buf, 0, blufi_data->alloc_len);
    }
    log_debug("\r\n====length:%d   \r\n", length);
    memcpy(&blufi_data->buf[blufi_data->recv_offset], data, length);
    blufi_data->recv_offset += length;
    //check the data
    if (data[length - 1] == '}')
    {
        return 0xff;
    }
    return 0;
}

int bt_stack_blufi_send(uint8_t *string, uint32_t length)
{
    memcpy(counter_string, string, length);
    counter_string_len = length;
    if (le_notification_enabled)
    {
        log_debug("\r\n===start send string====\r\n");
        send_buffer_complete = 0;
        send_buffer_index = 0;
    }
    else
    {
        log_error("\r\n===le_notification_enabled:%d can't send====\r\n", le_notification_enabled);
        return -1;
    }
    return 0;
}

void bt_send_api(void)
{
    uint8_t wifi_status = 1;
    uint8_t ip_address[4] = {192, 168, 1, 1};
    char temp_string[100] = {0};
    uint32_t temp_length = 0;
    // counter_string_len = sprintf(counter_string, "{wifi:'%s'}", wifi_status?"on":"off");
    temp_length = sprintf(temp_string, "{wifi:'%s', url:' http://%d.%d.%d.%d/index.html'}", wifi_status ? "on" : "off", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    log_debug("\r\n======counter_string_len:%d======\r\n", temp_length);

    bt_stack_blufi_send((uint8_t *)temp_string, temp_length);
}
MSH_CMD_EXPORT(bt_send_api, send data);

static void heartbeat_handler(struct btstack_timer_source *ts)
{
    if (send_buffer_complete == 1)
    {
        //tx complete no need
        //printf("\r\n === tx complete===\r\n ");
        btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
        btstack_run_loop_add_timer(ts);
        return;
    }
    if ((le_notification_enabled) && (counter_string_len != 0))
    {
        att_server_request_can_send_now_event(con_handle);
    }

    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
}
/* LISTING_END */

/* 
 * @section Packet Handler
 *
 * @text The packet handler is used to:
 *        - stop the counter after a disconnect
 *        - send a notification when the requested ATT_EVENT_CAN_SEND_NOW is received
 */

/* LISTING_START(packetHandler): Packet Handler */
static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);
    uint8_t send_length = 0;

    switch (packet_type)
    {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet))
        {
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            le_notification_enabled = 0;
            break;
        case ATT_EVENT_CAN_SEND_NOW:
            if ((counter_string_len - send_buffer_index) > GATT_SEND_MAX_SIZE)
            {
                send_length = GATT_SEND_MAX_SIZE;
            }
            else
            {
                send_length = counter_string_len - send_buffer_index;
            }
            att_server_notify(con_handle, ATT_CHARACTERISTIC_FF01_01_VALUE_HANDLE, (uint8_t *)(counter_string + send_buffer_index), send_length);
            send_buffer_index += send_length;
            log_debug("\r\n====send length= %d ==left:%s===\r\n", send_length, (uint8_t *)(counter_string + send_buffer_index));
            if (send_buffer_index >= counter_string_len)
            {
                //send complete
                send_buffer_complete = 1;
            }
            break;
        }
        break;
    }
}
/* LISTING_END */

/*
 * @section ATT Read
 *
 * @text The ATT Server handles all reads to constant data. For dynamic data like the custom characteristic, the registered
 * att_read_callback is called. To handle long characteristics and long reads, the att_read_callback is first called
 * with buffer == NULL, to request the total value length. Then it will be called again requesting a chunk of the value.
 * See Listing attRead.
 */

/* LISTING_START(attRead): ATT Read */

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    //    UNUSED(connection_handle);
    uint8_t string[] = {192, 168, 0, 1};
    log_debug("\r\n read the att %x, %x, offset:%x,buffer:%p, buffer_size:%x\r\n", connection_handle, att_handle, offset, buffer, buffer_size);

    if (att_handle == ATT_CHARACTERISTIC_FF01_01_VALUE_HANDLE)
    {
        return att_read_callback_handle_blob((const uint8_t *)string, 4, offset, buffer, buffer_size);
    }
    return 0;
}
/* LISTING_END */

/*
 * @section ATT Write
 *
 * @text The only valid ATT write in this example is to the Client Characteristic Configuration, which configures notification
 * and indication. If the ATT handle matches the client configuration handle, the new configuration value is stored and used
 * in the heartbeat handler to decide if a new value should be sent. See Listing attWrite.
 */

/* LISTING_START(attWrite): ATT Write */
static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(buffer_size);

    log_debug("\r\n write the att %x, %x, offset:%x,buffer:%p, buffer_size:%x transaction_mode %d \r\n", connection_handle, att_handle, offset, buffer, buffer_size, transaction_mode);
    if (att_handle == ATT_CHARACTERISTIC_FF01_01_CLIENT_CONFIGURATION_HANDLE)
    {
        le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
        log_debug("\r\n deal with the notify le_notification_enabled:%d %x, %x\r\n", le_notification_enabled, buffer[0], buffer[1]);
    }
    if (att_handle == ATT_CHARACTERISTIC_FF01_01_VALUE_HANDLE)
    {
        log_debug("\r\n recv data: %s length :%d\r\n", buffer, buffer_size);
        uint8_t ret = bsal_blufi_push_data(&blufi_data, buffer_size, buffer);
        if (ret == 0xff)
        {
            if (!wifi_is_ready())
            {
                wifi_connect((char *)blufi_data.buf);
            }
            else
            {
                int retry_cnt = 3;
                char *wifi_status = wifi_status_get();

                while (retry_cnt)
                {
                    if (bt_stack_blufi_send((uint8_t *)wifi_status, strlen(wifi_status)) < 0)
                    {
                        retry_cnt--;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            //the data is ready
            log_debug("\r\n BLUFI: THE RECEIVE DATA IS :%s \r\n", blufi_data.buf);
        }
    }

    con_handle = connection_handle;
    return 0;
}
/* LISTING_END */

int btstack_main(void);
int btstack_main(void)
{
    le_counter_setup();
    blufi_data.alloc_len = BLUFI_RECV_BUFF_SIZE;
    blufi_data.recv_offset = 0;

    // turn on!
    hci_power_control(HCI_POWER_ON);
    return 0;
}
/* EXAMPLE_END */
