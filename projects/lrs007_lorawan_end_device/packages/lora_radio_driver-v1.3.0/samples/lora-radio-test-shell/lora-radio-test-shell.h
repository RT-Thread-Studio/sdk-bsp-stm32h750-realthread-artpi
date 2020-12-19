/*!
 * \file      lora-radio-test-shell.h
 *
 * \brief     lora radio test implementation
 *
 * \copyright SPDX-License-Identifier: Apache-2.0
 * 
 * \author    Forest-Rain
 */

#ifndef __LORA_RADIO_TEST_SHELL_H__
#define __LORA_RADIO_TEST_SHELL_H__


#if defined( PHY_REGION_AS923 )

#define RF_FREQUENCY                                923000000 // Hz

#elif defined( PHY_REGION_AU915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( PHY_REGION_CN470 ) || defined ( PHY_REGION_CN470S )

#define RF_FREQUENCY                                470300000 // Hz

#elif defined( PHY_REGION_CN779 )

#define RF_FREQUENCY                                779000000 // Hz

#elif defined( PHY_REGION_EU433 )

#define RF_FREQUENCY                                433000000 // Hz

#elif defined( PHY_REGION_EU868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( PHY_REGION_KR920 )

#define RF_FREQUENCY                                920000000 // Hz

#elif defined( PHY_REGION_IN865 )

#define RF_FREQUENCY                                865000000 // Hz

#elif defined( PHY_REGION_US915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( PHY_REGION_RU864 )

#define RF_FREQUENCY                                864000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE          false
#define LORA_IQ_INVERSION_ON_DISABLE                false


#define FSK_FDEV                                    25000     // Hz
#define FSK_DATARATE                                50000     // bps

#if defined( LORA_RADIO_DRIVER_USING_LORA_CHIP_SX127X )

#define FSK_BANDWIDTH                               50000     // Hz >> SSB in sx127x
#define FSK_AFC_BANDWIDTH                           83333     // Hz

#elif defined( LORA_RADIO_DRIVER_USING_LORA_CHIP_SX126X)

#define FSK_BANDWIDTH                               100000    // Hz >> DSB in sx126x
#define FSK_AFC_BANDWIDTH                           166666    // Hz >> Unused in sx126x

#else
    #error "Please define a lora-shield in the compiler options."
#endif

#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here

#define LORA_MASTER_DEVADDR 0x11223344
#define LORA_SLAVER_DEVADDR 0x01020304
#define MAC_HEADER_OVERHEAD 13

// Ping pong event
#define EV_RADIO_INIT            0x0001
#define EV_RADIO_TX_START        0x0002
#define EV_RADIO_TX_DONE         0x0004
#define EV_RADIO_TX_TIMEOUT      0x0008
#define EV_RADIO_RX_DONE         0x0010
#define EV_RADIO_RX_TIMEOUT      0x0020
#define EV_RADIO_RX_ERROR        0x0040
#define EV_RADIO_ALL             (EV_RADIO_INIT | EV_RADIO_TX_START | EV_RADIO_TX_DONE | EV_RADIO_TX_TIMEOUT | EV_RADIO_RX_DONE | EV_RADIO_RX_TIMEOUT | EV_RADIO_RX_ERROR)

typedef struct 
{
    RadioModems_t modem; // LoRa Modem \ FSK modem
    uint32_t frequency;
    int8_t txpower;
    
    // LoRa
    uint8_t sf;    // spreadfactor
    uint8_t bw;    // bandwidth
    uint8_t cr;    // coderate

    // FSK
    uint32_t fdev;
    uint32_t datarate;
    uint32_t fsk_bandwidth;
    uint32_t fsk_afc_bandwidth;
    uint16_t preamble_len;

}lora_radio_test_t;

#endif

