/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *                forest-rain
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __LORAWAN_ED_APS_H__
#define __LORAWAN_ED_APS_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "Commissioning.h"
#include "LoRaMac.h"
#include "region/Region.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

#define HEX16(X)  X[0],X[1],X[2],X[3],X[4],X[5],X[6],X[7],X[8],X[9],X[10],X[11],X[12],X[13],X[14],X[15]
#define HEX8(X)   X[0],X[1],X[2],X[3],X[4],X[5],X[6],X[7]

/*!
 * Application Data structure
 */
typedef struct
{
    /*point to the LoRa App data buffer*/
    uint8_t* Buff;
    /*LoRa App data buffer size*/
    uint8_t BuffSize;
    /*Port on which the LoRaWAN App is data is sent/ received*/
    uint8_t Port;
    /* downlink info */
    /* Rssi of the received packet */
    int16_t Rssi;
    /* Snr of the received packet */
    int8_t Snr;
    /* Set if an acknowledgement was received */
    bool AckReceived;
    /* The downlink counter value for the received frame */
    uint32_t DownLinkCounter;

} lorawan_ed_appdata_t;

typedef enum 
{
  LORAWAN_ED_NOT_JOIN_NETWORK = 0, 
  LORAWAN_ED_JOINED_NETWORK,
} lorawan_ed_join_status_t;

typedef enum 
{
  LORAWAN_ED_STATUS_ERROR = -1, 
  LORAWAN_ED_STATUS_SUCCESS = 0
} lorawan_ed_error_status_t;

typedef enum 
{
  LORAWAN_ED_DATA_MESSAGE_TYPE_UNCONFIRMED = 0,
  LORAWAN_ED_DATA_MESSAGE_TYPE_CONFIRMED,
} lorawan_ed_confirm_t;

typedef enum
{
  LORAWAN_ED_ACTIVATION_TYPE_OTAA = 0,
  LORAWAN_ED_ACTIVATION_TYPE_ABP,
} lorawan_ed_activation_type_t;

typedef struct
{
    /*!
     * nvm flag
     */
    uint16_t nvm_flag;
    /*!
     * LoRaWAN device class
     */
    DeviceClass_t Class;
    /*!
     * @brief Activation type
     */
    bool ActivationType;
    /*!
     * @brief Data Message type,if unconfirm is false
     */
    bool DataMessageType;
    /*!
     * @brief Activation state of adaptativeDatarate
     */
    bool AdrEnable;
    /*!
     * @brief Uplink datarate, if AdrEnable is off
     */
    int8_t TxDatarate;
    /*!
     * @brief Enable or disable a public network
     *
     */
    bool EnablePublicNetwork;

} lorawan_ed_params_t;
#define LORAWAN_ED_APS_CFG_SIZE sizeof(lorawan_ed_params_t)

typedef struct
{
    /*!
     * nvm flag
     */
    uint16_t nvm_flag;
    /* EUI */
    uint8_t DevEui[8];
    uint8_t JoinEui[8];

    /* OTAA */
    uint8_t AppKey[16];
    /* 1.1.x OTAA */
    uint8_t NwkKey[16];

    /* ABP */
    uint32_t DevAddr;
    uint8_t AppSKey[16];
    uint8_t NwkSEncKey[16];

    /* 1.1.x ABP */
    uint8_t FNwkSIntKey[16];
    uint8_t SNwkSIntKey[16];

} lorawan_ed_device_id_info_t;
#define LORAWAN_ED_DEV_ID_INFO_SIZE sizeof(lorawan_ed_device_id_info_t)

/* lorawan end-device app callbacks*/
typedef struct
{
    /*!
     * @brief Get the current battery level
     *
     * @retval value  battery level ( 0: very low, 254: fully charged )
     */
    uint8_t ( *BoardGetBatteryLevel )( void );
    /*!
     * \brief Get the current temperature
     *
     * \retval value  temperature in degreeCelcius( q7.8 )
     */
    uint16_t ( *BoardGetTemperatureLevel)( void );
    /*!
     * @brief callback indicating End Device has joined
     *
     * @param [IN] None
     */
    void ( *lorawan_ed_joined )( void );
    /*!
     * @brief Confirm the tx finished
     *
     * @param [IN] status
     *
     */
    void ( *lorawan_ed_tx_confirm ) ( McpsConfirm_t *mcpsConfirm );
    /*!
     * @brief Process Rx Data received from lorawan network
     *
     * @param [IN] AppData structure
     *
     */
    void ( *lorawan_ed_receive_message ) ( lorawan_ed_appdata_t *AppData);
    /*!
     * @brief Process link check received from lorawan network 
     *
     * @param [IN] MlmeConfirm_t *mlmeConfirm
     */
    void ( *lorawan_ed_receive_linkcheck ) ( MlmeConfirm_t *mlmeConfirm );   
    /*!
     * @brief Confirms the class change 
     *
     * @param [IN] AppData is a buffer to process
     *
     * @param [IN] port is a Application port on wicth Appdata will be sent
     *
     * @param [IN] length is the number of recieved bytes
     */
    void ( *lorawan_ed_confirm_device_class) ( DeviceClass_t Class );
    /*!
     * @brief callback indicating an uplink transmission is needed to allow
     *        a pending downlink transmission 
     *
     * @param [IN] None
     */
    void ( *lorawan_ed_tx_dummy) ( void );
 
//#ifndef LORAWAN_ED_STACK_USING_ON_RTOS_RT_THREAD
    /*!
     *\brief    Will be called each time a Radio IRQ is handled by the MAC
     *          layer.
     * 
     *\warning  Runs in a IRQ context. Should only change variables state.  
     */
    void ( *MacProcessNotify )( void );
//#endif
} lorawan_ed_app_callback_t;

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/**
 * @brief lorawan end-device Initialisation
 * @param [IN] lorawan_ed_app_callback_t *callbacks
 * @retval none
 */
void lorawan_ed_stack_init(lorawan_ed_app_callback_t *callbacks);

/**
 * @brief Join a lorawan Network
 * @Note if the device is ABP, this is a pass through functon
 * @param [IN] none
 * @retval LoRaMacStatus
 */
LoRaMacStatus_t lorawan_ed_start_join_network( void );

/**
 * @brief update the device join status,when the device is joined to the network
 * @param [IN] none
 * @retval returns join status
 */
lorawan_ed_join_status_t lorawan_ed_join_status( void );

/**
 * @brief change lorawan device class type
 * @Note callback lorawan_ed_request_device_class request to change device class
 * @Note Only switch from class A to class B/C OR from  class B/C to class A is allowed
 * @param [IN] DeviceClass_t NewClass
 * @retval lorawan_ed_error_status_t
 */
lorawan_ed_error_status_t lorawan_ed_request_device_class( DeviceClass_t newClass );

/**
 * @brief class A send data
 * @param [IN] lorawan_ed_appdata_t* AppData
 * @retval send result
 */
bool lorawan_ed_send(lorawan_ed_appdata_t* AppData);

/**
 * @brief set deveui
 * @param [IN] *deveui
 * @retval  result
 */
uint8_t lorawan_ed_set_deveui(uint8_t *deveui);

/**
 * @brief get deveui
 * @param [IN] none
 * @retval  deveui
 */
uint8_t* lorawan_ed_get_deveui(void);

/**
 * @brief set joineui
 * @param [IN] *joineui
 * @retval  result
 */
uint8_t lorawan_ed_set_joineui(uint8_t *joineui);

/**
 * @brief get joineui
 * @param [IN] none
 * @retval  joineui
 */
uint8_t* lorawan_ed_get_joineui(void);

/**
 * @brief set appkey
 * @param [IN] *appkey
 * @retval  result
 */
uint8_t lorawan_ed_set_appkey(uint8_t *appkey);

/**
 * @brief get appkey
 * @param [IN] none
 * @retval  appkey
 */
uint8_t* lorawan_ed_get_appkey(void);

/**
 * @brief set devaddr
 * @param [IN] *devaddr
 * @retval  result
 */
uint8_t lorawan_ed_set_devaddr(uint8_t *devaddr);

/**
 * @brief get devaddr
 * @param [IN] none
 * @retval  devaddr
 */
uint32_t lorawan_ed_get_devaddr(void);

/**
 * @brief set appskey
 * @param [IN] *appkey
 * @retval  result
 */
uint8_t lorawan_ed_set_appskey(uint8_t *appskey);

/**
 * @brief get appskey
 * @param [IN] none
 * @retval  appkey
 */
uint8_t* lorawan_ed_get_appskey(void);

/**
 * @brief set nwkskey
 * @param [IN] *appkey
 * @retval  result
 */
uint8_t lorawan_ed_set_nwkskey(uint8_t *nwkskey);

/**
 * @brief get nwkskey
 * @param [IN] none
 * @retval  appkey
 */
uint8_t* lorawan_ed_get_nwkskey(void);

#ifdef PKG_USING_EASYFLASH
/**
 * @brief save device info
 * @param [IN] none
 * @retval  none
 */
void lorawan_ed_save_dev_info(void);

/**
 * @brief get device info
 * @param [IN] none
 * @retval  len
 */
int lorawan_ed_get_nvm_dev_info(void);

/**
 * @brief save configuration info
 * @param [IN] none
 * @retval  none
 */
void lorawan_ed_save_cfg(void);

/**
 * @brief get configuration info
 * @param [IN] none
 * @retval  len
 */
int lorawan_ed_get_nvm_cfg(void);
#endif


/**
 * @brief for printf loramac status
 */
extern const char* loramac_status_strings[];

#ifdef __cplusplus
}
#endif

#endif /*__LORAWAN_ED_APS_H__*/
