/*!
 * \file      lgd-test-shell.h
 *
 * \brief     lora gw driver test implementation
 *
 * \copyright SPDX-License-Identifier: Apache-2.0
 *
 * \author    Forest-Rain
 */

#ifndef __LORA_GW_DRIVER_TESTER_H__
#define __LORA_GW_DRIVER_TESTER_H__

#ifndef LGD_DBG_SHELL_TEST
    #define LGD_DBG_SHELL_TEST                          0
#endif

#ifndef LGD_DBG_SHELL_RX_INFO
    #define LGD_DBG_SHELL_RX_INFO                       0
#endif

#if defined( LORA_GW_DRIVER_USING_PHY_REGION_AS923 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_AS923 )

    #define RF_FREQUENCY                                923000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_AU915 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_AU915 )

    #define RF_FREQUENCY                                915000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_CN470 ) || defined ( LORA_GW_DRIVER_USING_PHY_REGION_CN470S ) || \
      defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_CN470 ) || defined ( LORA_GW_DRIVER_LIB_USING_PHY_REGION_CN470S )

    #define RF_FREQUENCY                                470300000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_CN779 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_CN779 )

    #define RF_FREQUENCY                                779000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_EU433 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_EU433 )

    #define RF_FREQUENCY                                433000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_EU868 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_EU868 )

    #define RF_FREQUENCY                                868000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_KR920 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_KR920 )

    #define RF_FREQUENCY                                920000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_IN865 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_IN865 )

    #define RF_FREQUENCY                                865000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_US915 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_US915 )

    #define RF_FREQUENCY                                915000000 // Hz

#elif defined( LORA_GW_DRIVER_USING_PHY_REGION_RU864 ) || defined( LORA_GW_DRIVER_LIB_USING_PHY_REGION_RU864 )

    #define RF_FREQUENCY                                864000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_RX_FREQUENCE_OFFSET            0  // 0       TX Freq = RX Freq
                                             // 1800000 TX Freq = RX Freq + 1.8M   

#define LORA_MASTER_DEVADDR               0x11223344
#define LORA_SLAVER_DEVADDR               0x01020304
#define MAC_HEADER_OVERHEAD               13
#define MIN_TETS_APP_DATA_SIZE            17 // for PING protocol

// lora gw driver test state
#define LGD_TEST_AS_CONCENTRATOR_MODE     0x00
#define LGD_TEST_AS_DEVICE_MODE           0x01
#define LGD_TEST_AS_CW_MODE               0x02

#endif

