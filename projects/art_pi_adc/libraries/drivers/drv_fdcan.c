/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-24     heyuan       the first version
 * 2020-08-17     malongwei    Fix something
 */

#include "board.h"
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_CAN

#if defined(BSP_USING_FDCAN1) || defined(BSP_USING_FDCAN2)

#include "drv_fdcan.h"

//#define DRV_DEBUG
#define LOG_TAG    "drv_can"
#include <drv_log.h>

#ifdef BSP_USING_FDCAN1
static _stm32_fdcan_t st_DrvCan1=
{
	.name = "fdcan1",
	.fdcanHandle.Instance = FDCAN1,
};
#endif

#ifdef BSP_USING_FDCAN2
static _stm32_fdcan_t st_DrvCan2=
{
	.name = "fdcan2",
	.fdcanHandle.Instance = FDCAN2,
};
#endif


static const _stm32_fdcan_NTconfig_t st_CanNTconfig[]=
/*baud brp sjw tseg1 tseg2*/
{
	{CAN1MBaud,     10,8,15,4},
	{CAN800kBaud,   10,8,20,4},
	{CAN500kBaud,   20,8,15,4},
	{CAN250kBaud,   20,8,35,4},
	{CAN125kBaud,   40,8,35,4},
	{CAN100kBaud,   40,8,44,5},
	{CAN50kBaud,    80,8,44,5},
	{CAN20kBaud,    200,8,44,5},
	{CAN10kBaud,    400,8,44,5}
};

/*
*function name:_inline_get_NTbaud_index
*Inf: get can normal transmit baud rate config index
*
*#param baud_rate:decimalism numeral
*
*#return config index
*/
static uint32_t _inline_get_NTbaud_index(uint32_t baud_rate)
{
    uint32_t len, index;

    len = sizeof(st_CanNTconfig) / sizeof(st_CanNTconfig[0]);
	
    for (index = 0; index < len; index++)
    {
        if (st_CanNTconfig[index].u32NBaudrate == baud_rate)

		return index;
    }

    return 0;
}

static void _inline_can_tx_header_init(_stm32_fdcan_t *pCan)
{
	  /* Prepare Tx Header */
	  pCan->TxHeader.Identifier = 0x000000;					
	  pCan->TxHeader.IdType = FDCAN_EXTENDED_ID;			
	  pCan->TxHeader.TxFrameType = FDCAN_DATA_FRAME;		
	  pCan->TxHeader.DataLength = FDCAN_DLC_BYTES_8;		
	  pCan->TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	  pCan->TxHeader.BitRateSwitch = FDCAN_BRS_OFF;			
	  pCan->TxHeader.FDFormat = FDCAN_CLASSIC_CAN;			
	  pCan->TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	  pCan->TxHeader.MessageMarker = 0;
}

static rt_err_t _inline_can_config(struct rt_can_device *can, struct can_configure *cfg)
{
	_stm32_fdcan_t *pdrv_can;
	rt_uint32_t tmp_u32Index;

	RT_ASSERT(can);
	RT_ASSERT(cfg);

	pdrv_can = (_stm32_fdcan_t *)can->parent.user_data;

	RT_ASSERT(pdrv_can);

	pdrv_can->fdcanHandle.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	pdrv_can->fdcanHandle.Init.Mode = FDCAN_MODE_NORMAL;
	pdrv_can->fdcanHandle.Init.AutoRetransmission = DISABLE;
	pdrv_can->fdcanHandle.Init.TransmitPause = DISABLE;
	pdrv_can->fdcanHandle.Init.ProtocolException = DISABLE;

	switch (cfg->mode)
	{
	case RT_CAN_MODE_NORMAL:
		pdrv_can->fdcanHandle.Init.Mode = FDCAN_MODE_NORMAL;
		break;
	case RT_CAN_MODE_LISEN:
		pdrv_can->fdcanHandle.Init.Mode = FDCAN_MODE_BUS_MONITORING;
		break;
	case RT_CAN_MODE_LOOPBACK:
		pdrv_can->fdcanHandle.Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
		break;
	default:
		pdrv_can->fdcanHandle.Init.Mode = FDCAN_MODE_NORMAL;
		break;
	}

	/*config baud rate*/
	tmp_u32Index = _inline_get_NTbaud_index(cfg->baud_rate);

	pdrv_can->fdcanHandle.Init.NominalPrescaler = st_CanNTconfig[tmp_u32Index].u16Nbrp;
	pdrv_can->fdcanHandle.Init.NominalSyncJumpWidth = st_CanNTconfig[tmp_u32Index].u8Nsjw;
	pdrv_can->fdcanHandle.Init.NominalTimeSeg1 = st_CanNTconfig[tmp_u32Index].u8Ntseg1;
	pdrv_can->fdcanHandle.Init.NominalTimeSeg2 = st_CanNTconfig[tmp_u32Index].u8Ntseg2;


	if(pdrv_can->fdcanHandle.Instance == FDCAN1)
	{
		pdrv_can->fdcanHandle.Init.MessageRAMOffset = 0;					
	}
	else
	{
		pdrv_can->fdcanHandle.Init.MessageRAMOffset = 1280;					
	}
	pdrv_can->fdcanHandle.Init.StdFiltersNbr = 2;							
	pdrv_can->fdcanHandle.Init.ExtFiltersNbr = 2;							
	pdrv_can->fdcanHandle.Init.RxFifo0ElmtsNbr = 1;							
	pdrv_can->fdcanHandle.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;		
	pdrv_can->fdcanHandle.Init.RxBuffersNbr = 0;							
	pdrv_can->fdcanHandle.Init.TxEventsNbr = 0;								
	pdrv_can->fdcanHandle.Init.TxBuffersNbr = 3;							
	pdrv_can->fdcanHandle.Init.TxFifoQueueElmtsNbr = 0;						
	pdrv_can->fdcanHandle.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;	
	pdrv_can->fdcanHandle.Init.TxElmtSize = FDCAN_DATA_BYTES_8;				

	if (HAL_FDCAN_Init(&pdrv_can->fdcanHandle) != HAL_OK)
	{
		return -RT_ERROR;
	}
	/* default filter config */
	HAL_FDCAN_ConfigFilter(&pdrv_can->fdcanHandle , &pdrv_can->FilterConfig);
	/*init fdcan tx header*/
	_inline_can_tx_header_init(pdrv_can);
	/* can start */
	HAL_FDCAN_Start(&pdrv_can->fdcanHandle);
	return RT_EOK;
}

static rt_err_t _inline_can_filter_config(_stm32_fdcan_t *pdrv_can,struct rt_can_filter_config *puser_can_filter_config)
{
	int tmp_i32IndexCount;
	RT_ASSERT(pdrv_can);
	RT_ASSERT(puser_can_filter_config);
	 /* get default filter */
	for (tmp_i32IndexCount = 0; tmp_i32IndexCount < puser_can_filter_config->count; tmp_i32IndexCount++)
	{
		pdrv_can->FilterConfig.FilterIndex = puser_can_filter_config->items[tmp_i32IndexCount].hdr;
		pdrv_can->FilterConfig.FilterID1 = puser_can_filter_config->items[tmp_i32IndexCount].id;
		pdrv_can->FilterConfig.FilterID2 = puser_can_filter_config->items[tmp_i32IndexCount].mask;
		if(puser_can_filter_config->items[tmp_i32IndexCount].ide == RT_CAN_EXTID)
		{
			pdrv_can->FilterConfig.IdType = FDCAN_EXTENDED_ID;
		}
		else
		{
			pdrv_can->FilterConfig.IdType = FDCAN_STANDARD_ID;
		}
		pdrv_can->FilterConfig.FilterType = FDCAN_FILTER_MASK;
		pdrv_can->FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
		if(HAL_FDCAN_ConfigFilter(&pdrv_can->fdcanHandle , &pdrv_can->FilterConfig) != HAL_OK)
		{
			return -RT_ERROR;
		}
	}
	return RT_EOK;
}

static rt_err_t _inline_can_control(struct rt_can_device *can, int cmd, void *arg)
{

	rt_uint32_t argval;
	_stm32_fdcan_t *pdrv_can;
	struct rt_can_filter_config *filter_cfg;

	RT_ASSERT(can != RT_NULL);
	pdrv_can = (_stm32_fdcan_t *)can->parent.user_data;
	RT_ASSERT(pdrv_can != RT_NULL);

	switch (cmd)
	{

	case RT_DEVICE_CTRL_CLR_INT:
		argval = (rt_uint32_t) arg;
		if (argval == RT_DEVICE_FLAG_INT_RX)
		{
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
		}
		else if (argval == RT_DEVICE_FLAG_INT_TX)
		{
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_TX_FIFO_EMPTY);
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_TX_COMPLETE);
		}
		else if (argval == RT_DEVICE_CAN_INT_ERR)
		{
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ERROR_WARNING);
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ERROR_PASSIVE);
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ERROR_LOGGING_OVERFLOW);
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_BUS_OFF);
			HAL_FDCAN_DeactivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ARB_PROTOCOL_ERROR);
		}
		break;
	case RT_DEVICE_CTRL_SET_INT:
		argval = (rt_uint32_t) arg;
		if (argval == RT_DEVICE_FLAG_INT_RX)
		{
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, FDCAN_INTERRUPT_LINE0);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

			if(FDCAN1 == pdrv_can->fdcanHandle.Instance)
			{
				HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 1);
				HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
			}
			else
			{
				HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 1);
				HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
			}

		}
		else if (argval == RT_DEVICE_FLAG_INT_TX)
		{
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_TX_COMPLETE, FDCAN_INTERRUPT_LINE1);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER1);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER2);

			if(FDCAN1 == pdrv_can->fdcanHandle.Instance)
			{
				HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 2);
				HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
			}
			else
			{
				HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 2);
				HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
			}
		}
		else if (argval == RT_DEVICE_CAN_INT_ERR)
		{
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_BUS_OFF, FDCAN_INTERRUPT_LINE1);
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_ERROR_WARNING, FDCAN_INTERRUPT_LINE1);
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_ERROR_PASSIVE, FDCAN_INTERRUPT_LINE1);
			HAL_FDCAN_ConfigInterruptLines(&pdrv_can->fdcanHandle, FDCAN_IT_ARB_PROTOCOL_ERROR, FDCAN_INTERRUPT_LINE1);

			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_BUS_OFF, 0);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ERROR_WARNING, 0);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ERROR_PASSIVE, 0);
			HAL_FDCAN_ActivateNotification(&pdrv_can->fdcanHandle,  FDCAN_IT_ARB_PROTOCOL_ERROR, 0);
			if(FDCAN1 == pdrv_can->fdcanHandle.Instance)
			{
				HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 2);
				HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
			}
			else
			{
				HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 2);
				HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
			}
		}
		break;
	case RT_CAN_CMD_SET_FILTER:
		if (RT_NULL == arg)
		{
			/* default filter config */
			HAL_FDCAN_ConfigFilter(&pdrv_can->fdcanHandle , &pdrv_can->FilterConfig);
		}
		else
		{
			filter_cfg = (struct rt_can_filter_config *)arg;
			_inline_can_filter_config(pdrv_can, filter_cfg);
		}
		break;
	case RT_CAN_CMD_SET_MODE:
		argval = (rt_uint32_t) arg;
		if (argval != RT_CAN_MODE_NORMAL &&
			argval != RT_CAN_MODE_LISEN &&
			argval != RT_CAN_MODE_LOOPBACK &&
			argval != RT_CAN_MODE_LOOPBACKANLISEN)
		{
			return -RT_ERROR;
		}
		if (argval != pdrv_can->device.config.mode)
		{
			pdrv_can->device.config.mode = argval;
			return _inline_can_config(&pdrv_can->device, &pdrv_can->device.config);
		}
		break;
	case RT_CAN_CMD_SET_BAUD:
		argval = (rt_uint32_t ) arg;
		/*just low to 50kbit/s*/
		if (argval != CAN1MBaud &&
			argval != CAN800kBaud &&
			argval != CAN500kBaud &&
			argval != CAN250kBaud &&
			argval != CAN125kBaud &&
			argval != CAN100kBaud &&
			argval != CAN50kBaud  &&
			argval != CAN20kBaud  &&
			argval != CAN10kBaud)
		{
			return -RT_ERROR;
		}
		if (argval != pdrv_can->device.config.baud_rate)
		{
			pdrv_can->device.config.baud_rate = argval;
			return _inline_can_config(&pdrv_can->device, &pdrv_can->device.config);
		}
		break;

	case RT_CAN_CMD_SET_PRIV:
		argval = (rt_uint32_t) arg;
		if (argval != RT_CAN_MODE_PRIV &&
				argval != RT_CAN_MODE_NOPRIV)
		{
			return -RT_ERROR;
		}
		if (argval != pdrv_can->device.config.privmode)
		{
			pdrv_can->device.config.privmode = argval;

			return RT_EOK;
		}
		break;
	case RT_CAN_CMD_GET_STATUS:
	{
		rt_uint32_t tmp_u32Errcount;
		rt_uint32_t tmp_u32status;
		tmp_u32Errcount = pdrv_can->fdcanHandle.Instance->ECR;
		tmp_u32status = pdrv_can->fdcanHandle.Instance->PSR;

		pdrv_can->device.status.rcverrcnt = (tmp_u32Errcount>>8)&0x000000ff;
		pdrv_can->device.status.snderrcnt = (tmp_u32Errcount)&0x000000ff;
		pdrv_can->device.status.lasterrtype = tmp_u32status&0x000000007;

		rt_memcpy(arg, &pdrv_can->device.status, sizeof(pdrv_can->device.status));
	}
	break;
	}

	return RT_EOK;
}

static int _inline_can_sendmsg(struct rt_can_device *can, const void *buf, rt_uint32_t box_num)
{

	_stm32_fdcan_t *pdrv_can;
	struct rt_can_msg *pmsg;
	uint32_t tmp_u32DataLen;
	RT_ASSERT(can);
	RT_ASSERT(buf);

	pdrv_can = (_stm32_fdcan_t *)can->parent.user_data;

	RT_ASSERT(pdrv_can);

	pmsg = (struct rt_can_msg *) buf;

	/* Check the parameters */
	if(pmsg->len > 8)
	{
		tmp_u32DataLen = 8;
	}
	else
	{
		tmp_u32DataLen = pmsg->len;
	}
	tmp_u32DataLen <<=16;

	if(pmsg->ide == RT_CAN_EXTID)
	{
		pdrv_can->TxHeader.IdType = FDCAN_EXTENDED_ID;
	}
	else
	{
		pdrv_can->TxHeader.IdType = FDCAN_STANDARD_ID;
	}
	if (RT_CAN_DTR == pmsg->rtr)
	{
		pdrv_can->TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	}
	else
	{
		pdrv_can->TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
	}

	pdrv_can->TxHeader.Identifier = pmsg->id;
	pdrv_can->TxHeader.DataLength = tmp_u32DataLen;
	if(HAL_FDCAN_AddMessageToTxBuffer(&pdrv_can->fdcanHandle, &pdrv_can->TxHeader, pmsg->data, FDCAN_TX_BUFFER0+box_num) != HAL_OK)
	{
		return -RT_ERROR;
	}
	else
	{
		/* Request transmission */
		HAL_FDCAN_EnableTxBufferRequest(&pdrv_can->fdcanHandle,FDCAN_TX_BUFFER0+box_num);
		return RT_EOK;
	}
}

static int _inline_can_recvmsg(struct rt_can_device *can, void *buf, rt_uint32_t fifo)
{

    struct rt_can_msg *pmsg;
    _stm32_fdcan_t *pdrv_can;

    RT_ASSERT(can);
    RT_ASSERT(buf);

    pdrv_can = (_stm32_fdcan_t *)can->parent.user_data;
    pmsg = (struct rt_can_msg *) buf;
    if(HAL_FDCAN_GetRxMessage(&pdrv_can->fdcanHandle,FDCAN_RX_FIFO0+fifo, &pdrv_can->RxHeader, pmsg->data) != HAL_OK)
    {
    	return -RT_ERROR;
    }
    else
    {
    	if(pdrv_can->RxHeader.IdType == FDCAN_EXTENDED_ID)
    	{
    		pmsg->ide = RT_CAN_EXTID;
    	}
    	else
    	{
    		pmsg->ide = RT_CAN_STDID;
    	}

    	if(pdrv_can->RxHeader.RxFrameType == FDCAN_DATA_FRAME)
    	{
    		pmsg->rtr = RT_CAN_DTR;
		}
		else
		{
			pmsg->rtr = RT_CAN_RTR;
		}
    	pmsg->id = pdrv_can->RxHeader.Identifier;

    	pmsg->len = (pdrv_can->RxHeader.DataLength>>16)&0x0f;

    	pmsg->hdr = pdrv_can->RxHeader.FilterIndex;
    	return RT_EOK;
    }
}

static const struct rt_can_ops _can_ops =
{
    _inline_can_config,
    _inline_can_control,
    _inline_can_sendmsg,
    _inline_can_recvmsg,
};

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  	{
  		if(hfdcan->Instance == FDCAN1)
  		{
#ifdef BSP_USING_FDCAN1
  			//CAN1
  			/* Retreive Rx messages from RX FIFO0 */
  			rt_hw_can_isr(&st_DrvCan1.device, RT_CAN_EVENT_RX_IND | 0 << 8);
#endif
  		}
		else
		{
#ifdef BSP_USING_FDCAN2
			//CAN2
			/* Retreive Rx messages from RX FIFO0 */
			rt_hw_can_isr(&st_DrvCan2.device, RT_CAN_EVENT_RX_IND | 0 << 8);
#endif
		}
	}
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
	if(hfdcan->Instance == FDCAN1)
	{
#ifdef BSP_USING_FDCAN1
		//can1
		rt_hw_can_isr(&st_DrvCan1.device, RT_CAN_EVENT_TX_DONE | ((BufferIndexes-1) << 8));
#endif
	}
	else
	{
#ifdef BSP_USING_FDCAN2
		//can2
		rt_hw_can_isr(&st_DrvCan2.device, RT_CAN_EVENT_TX_DONE | ((BufferIndexes-1) << 8));
#endif
	}

}


void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
	if(hfdcan->Instance == FDCAN1)
	{
		//can1
	}
	else
	{
		//can2
	}
}

void HAL_FDCAN_TxBufferAbortCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{

}

void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
	rt_uint32_t tmp_u32Errcount;
	rt_uint32_t tmp_u32status;
	uint32_t ret = HAL_FDCAN_GetError(hfdcan);

	if(hfdcan->Instance == FDCAN1)
	{
#ifdef BSP_USING_FDCAN1
		//can1
		if(	(ret & FDCAN_IT_ARB_PROTOCOL_ERROR) &&
			(hfdcan->Instance->CCCR & FDCAN_CCCR_INIT_Msk))
		{
			//hfdcan->Instance->CCCR |= FDCAN_CCCR_CCE_Msk;
			hfdcan->Instance->CCCR &= ~FDCAN_CCCR_INIT_Msk;
			st_DrvCan1.device.status.errcode = 0xff;
		}
		else
		{
			tmp_u32Errcount = st_DrvCan1.fdcanHandle.Instance->ECR;
			tmp_u32status = st_DrvCan1.fdcanHandle.Instance->PSR;

			st_DrvCan1.device.status.rcverrcnt = (tmp_u32Errcount>>8)&0x000000ff;
			st_DrvCan1.device.status.snderrcnt = (tmp_u32Errcount)&0x000000ff;
			st_DrvCan1.device.status.lasterrtype = tmp_u32status&0x000000007;
		}
#endif /*BSP_USING_FDCAN1*/
	}
	else
	{
#ifdef BSP_USING_FDCAN2
		if(	(ret & FDCAN_IT_ARB_PROTOCOL_ERROR) &&
			(hfdcan->Instance->CCCR & FDCAN_CCCR_INIT_Msk))
		{
			//hfdcan->Instance->CCCR |= FDCAN_CCCR_CCE_Msk;
			hfdcan->Instance->CCCR &= ~FDCAN_CCCR_INIT_Msk;
			st_DrvCan2.device.status.errcode = 0xff;
		}
		else
		{
			//can2
			tmp_u32Errcount = st_DrvCan2.fdcanHandle.Instance->ECR;
			tmp_u32status = st_DrvCan2.fdcanHandle.Instance->PSR;
			st_DrvCan2.device.status.rcverrcnt = (tmp_u32Errcount>>8)&0x000000ff;
			st_DrvCan2.device.status.snderrcnt = (tmp_u32Errcount)&0x000000ff;
			st_DrvCan2.device.status.lasterrtype = tmp_u32status&0x000000007;
		}
#endif /*BSP_USING_FDCAN2*/
	}
}

#ifdef BSP_USING_FDCAN1

void FDCAN1_IT0_IRQHandler(void)             /* FDCAN2 interrupt line 0      */
{
	rt_interrupt_enter();
	HAL_FDCAN_IRQHandler(&st_DrvCan1.fdcanHandle);
	rt_interrupt_leave();
}

void FDCAN1_IT1_IRQHandler(void)             /* FDCAN2 interrupt line 1      */
{
	rt_interrupt_enter();
	HAL_FDCAN_IRQHandler(&st_DrvCan1.fdcanHandle);
	rt_interrupt_leave();
}

#endif /*BSP_USING_FDCAN1*/

#ifdef BSP_USING_FDCAN2
void FDCAN2_IT0_IRQHandler(void)             /* FDCAN2 interrupt line 0      */
{
	rt_interrupt_enter();
	HAL_FDCAN_IRQHandler(&st_DrvCan2.fdcanHandle);
	rt_interrupt_leave();
}

void FDCAN2_IT1_IRQHandler(void)             /* FDCAN2 interrupt line 1      */
{
	rt_interrupt_enter();
	HAL_FDCAN_IRQHandler(&st_DrvCan2.fdcanHandle);
	rt_interrupt_leave();
}
#endif/*BSP_USING_FDCAN2*/


static int rt_hw_can_init(void)
{
    struct can_configure config;
    config.baud_rate = CAN250kBaud;
    config.msgboxsz = 48;
    config.sndboxnumber = 1;
    config.mode = RT_CAN_MODE_NORMAL;
    config.privmode = RT_CAN_MODE_NOPRIV;
    config.ticks = 50;
    /* config default filter */
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0;
    sFilterConfig.FilterID2 = 0;

#ifdef BSP_USING_FDCAN1

	st_DrvCan1.FilterConfig = sFilterConfig;
	st_DrvCan1.device.config = config;

    /* register FDCAN1 device */
    rt_hw_can_register(&st_DrvCan1.device, st_DrvCan1.name, &_can_ops, &st_DrvCan1);
#endif /* BSP_USING_FDCAN1 */

#ifdef BSP_USING_FDCAN2
    st_DrvCan2.FilterConfig = sFilterConfig;
	st_DrvCan2.device.config = config;

	/* register FDCAN1 device */
	rt_hw_can_register(&st_DrvCan2.device, st_DrvCan2.name, &_can_ops, &st_DrvCan2);
#endif /* BSP_USING_FDCAN2 */

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_can_init);

#endif /* BSP_USING_FDCAN1 || BSP_USING_FDCAN2 */
#endif /* RT_USING_CAN */
