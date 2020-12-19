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

#ifndef __LORAWAN_CERTIFICATE_H__
#define __LORAWAN_CERTIFICATE_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
 /* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
   
#define LORAWAN_ED_CETIFICATE_TEST_FPORT 224
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
bool lorawan_ed_certificate_running(void);

void lorawan_ed_certificate_downlink_increment( void );

void lorawan_ed_certificate_linkcheck(MlmeConfirm_t *mlmeConfirm);

void lorawan_ed_certificate_rx( McpsIndication_t *mcpsIndication, MlmeReqJoin_t* JoinParameters);

#ifdef __cplusplus
}
#endif

#endif /*__LORAWAN_CERTIFICATE_H__*/
