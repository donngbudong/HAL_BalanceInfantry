#include "drv_can.h"
#include "Device.h"
#include "can.h"

/**
 *	@brief	CAN 标识符过滤器复位成默认配置
 */
static void CAN_Filter_Init(CAN_HandleTypeDef*hcan,CAN_FilterTypeDef*Filter_Can)
{
	Filter_Can->FilterMode = CAN_FILTERMODE_IDMASK;
	Filter_Can->FilterScale = CAN_FILTERSCALE_32BIT;
	Filter_Can->FilterIdHigh = 0x0000;
	Filter_Can->FilterIdLow = 0x0000;
	Filter_Can->FilterMaskIdHigh = 0x0000;
	Filter_Can->FilterMaskIdLow = 0x0000;
	Filter_Can->FilterBank = 0;
	Filter_Can->FilterFIFOAssignment = CAN_RX_FIFO0;
	Filter_Can->FilterActivation = ENABLE;
	Filter_Can->SlaveStartFilterBank=14;
	if(hcan->Instance==CAN1)
	{
	Filter_Can->FilterBank = 0;
	}
	else if(hcan->Instance==CAN2)
	{
	Filter_Can->FilterBank = 14;
	}
};

/**
 *	@brief	CAN1初始化
 */
void CAN1_Init(void)
{
  CAN_FilterTypeDef Filter_Can1;
  // 配置CAN标识符滤波器
	CAN_Filter_Init(&hcan1,&Filter_Can1);
	HAL_CAN_ConfigFilter(&hcan1,&Filter_Can1);
  // /激活CAN1 FIFO0 接收
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING); 
	// 开启CAN1
	HAL_CAN_Start(&hcan1);
}

/**
 *	@brief	CAN2初始化
 */
void CAN2_Init(void)
{
  CAN_FilterTypeDef Filter_Can2;
  // 配置CAN标识符滤波器
	CAN_Filter_Init(&hcan2,&Filter_Can2);
	//初始化CAN2过滤器
	HAL_CAN_ConfigFilter(&hcan2,&Filter_Can2);
  // /激活CAN2 FIFO0 接收
	HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
	 // 开启CAN2
	HAL_CAN_Start(&hcan2);
}

