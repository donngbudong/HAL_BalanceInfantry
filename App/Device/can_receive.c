#include "can_receive.h"
#include "Device.h"
//#include "chassis_task.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];

static CAN_TxHeaderTypeDef  gimbal_tx_message;
static uint8_t              gimbal_can_send_data[8];

static CAN_TxHeaderTypeDef  shoot_tx_message;
static uint8_t              shoot_can_send_data[8];

static CAN_TxHeaderTypeDef  rc_tx_message;
static uint8_t              rc_can_send_data[8];

static CAN_TxHeaderTypeDef  capacitance_tx_message;
static uint8_t              capacitance_can_send_data[8];

void CAN_MF9025_Decode(CAN_MF9025_DATE_T *str,uint8_t *Date)
{
	str->Last_Encoder = str->Encoder;
	str->Temp					  = (Date)[1];
	str->Torque_Current = (int16_t)((Date)[3] << 8 | (Date)[2]);
	str->Speed 					= (int16_t)((Date)[5] << 8 | (Date)[4]);
	str->Encoder				= ((uint16_t)(Date)[7]<< 8 | (Date)[6]);
}

void CAN_Date_Decode(CAN_GET_DATA_t *str, uint8_t *Data)                                    
{                         
	str->Motor_Angle = (uint16_t)((Data)[0] << 8 | (Data)[1]);            
	str->Motor_Speed = (uint16_t)((Data)[2] << 8 | (Data)[3]);      
	str->Motor_ELC   = (uint16_t)((Data)[4] << 8 | (Data)[5]);  
	str->Motor_Temp  = (Data)[6];                                   
}

void capacitance_date(super_capacitor_t *ptr, uint8_t *data)                                    
{                                                                   
		(ptr)->chi_voltage = (float)(uint16_t)((data)[0] << 8 | (data)[1]);            
		(ptr)->power = (float)(uint16_t)((data)[2] << 8 | (data)[3]);      
		(ptr)->rong_voltage = (uint16_t)((data)[4] << 8 | (data)[5]);  
		(ptr)->state = (data)[6];
		(ptr)->rong = (ptr)->rong_voltage*100/2400;
}


super_capacitor_t super_capacitor;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	static uint8_t rx_data[8];
	if(hcan->Instance==CAN1)
	{
	
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId)
    {
        case CAN_MF9025_1_ID:
						CAN_MF9025_Decode(&Chassis.Motor_Date[MF9025_R], rx_data);                                    
					break;
        case CAN_MF9025_2_ID:
						CAN_MF9025_Decode(&Chassis.Motor_Date[MF9025_L], rx_data);                                    
					break;
				case GIM_YAW_ID:
						CAN_Date_Decode(&Gimbal.YAW.Motor_Date.CAN_GetData, rx_data);                                    
					break;
				case 0X666://³¬µç
					capacitance_date(&super_capacitor,rx_data);
					break;
		}
	}
	if(hcan->Instance==CAN2)
	{
	
    HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &rx_header, rx_data);

    switch (rx_header.StdId)
    {
        case GIM_PIT_ID:
						CAN_Date_Decode(&Gimbal.PITCH.Motor_Date.CAN_GetData, rx_data);                                    
					break;

        case FRIC_L_ID:
						CAN_Date_Decode(&Shoot.Motor_Date[FRIC_L].Motor_Data.CAN_GetData, rx_data);
					break;

        case FRIC_R_ID:
						CAN_Date_Decode(&Shoot.Motor_Date[FRIC_R].Motor_Data.CAN_GetData, rx_data);
					break;

        case DRIVER_ID:
					CAN_Date_Decode(&Shoot.Motor_Date[DRIVER].Motor_Data.CAN_GetData, rx_data);
					break;
				case 0x567:
					RC_Ctrl.rc.ch0 = ((int16_t)(rx_data[0] << 8	| rx_data[1]));
					RC_Ctrl.rc.ch1 = ((int16_t)(rx_data[2] << 8 | rx_data[3]));
					RC_Ctrl.rc.ch2 = ((int16_t)(rx_data[4] << 8 | rx_data[5]));
					RC_Ctrl.rc.ch3 = ((int16_t)(rx_data[6] << 8 | rx_data[7]));
					break;
				case 0x520:
					RC_Ctrl.rc.s1 = ((uint8_t)(rx_data[0]));
					RC_Ctrl.rc.s2 = ((uint8_t)(rx_data[1]));

					RC_Ctrl.rc.sw = ((int16_t)(rx_data[2] << 8 	| rx_data[3]));
					RC_Ctrl.kb.key = ((int16_t)(rx_data[4] << 8 | rx_data[5]));
					break;
				
		}
  }
}



void CAN_cmd_capacitance(int16_t size)
{
	uint32_t send_mail_box;
	capacitance_tx_message.StdId = 0x333;
	capacitance_tx_message.IDE = CAN_ID_STD;
	capacitance_tx_message.RTR = CAN_RTR_DATA;
	capacitance_tx_message.DLC = 0x08;
	
	capacitance_can_send_data[0]=size>>8;
	capacitance_can_send_data[1]=size;
	
	HAL_CAN_AddTxMessage(&hcan1, &capacitance_tx_message, capacitance_can_send_data, &send_mail_box);
}
/**
* @brief µ×ÅÌµç»ú·¢ËÍ
* @param 
*/
void CAN_cmd_chassis(int16_t Right, int16_t Left)
{
	uint32_t send_mail_box;
	chassis_tx_message.StdId = CAN_MF9025_ALL_ID;
	chassis_tx_message.IDE = CAN_ID_STD;
	chassis_tx_message.RTR = CAN_RTR_DATA;
	chassis_tx_message.DLC = 0x08;
	
	chassis_can_send_data[0] = Right;
	chassis_can_send_data[1] = Right >> 8;
	chassis_can_send_data[2] = Left;
	chassis_can_send_data[3] = Left >>8;
//	chassis_can_send_data[4] = motor3 >> 8;
//	chassis_can_send_data[5] = motor3;
//	chassis_can_send_data[6] = motor4 >> 8;
//	chassis_can_send_data[7] = motor4;

	HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


void CAN_cmd_gimbal_yaw( int16_t yaw)
{
    uint32_t send_mail_box;
    gimbal_tx_message.StdId = GIMBAL_ALL_ID;
    gimbal_tx_message.IDE = CAN_ID_STD;
    gimbal_tx_message.RTR = CAN_RTR_DATA;
    gimbal_tx_message.DLC = 0x08;
    
    gimbal_can_send_data[0] = yaw >> 8;
    gimbal_can_send_data[1] = yaw;

    HAL_CAN_AddTxMessage(&hcan1, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
}


void CAN_cmd_gimbal_pitch(int16_t pitch,int16_t driver)
{
    uint32_t send_mail_box;
    gimbal_tx_message.StdId = GIMBAL_ALL_ID;
    gimbal_tx_message.IDE = CAN_ID_STD;
    gimbal_tx_message.RTR = CAN_RTR_DATA;
    gimbal_tx_message.DLC = 0x08;
    
    gimbal_can_send_data[2] = pitch >> 8;
    gimbal_can_send_data[3] = pitch;
		gimbal_can_send_data[4] = driver >> 8;
		gimbal_can_send_data[5] = driver;

    HAL_CAN_AddTxMessage(&hcan2, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
}


void CAN_cmd_shoot( int16_t shoot1,int16_t shoot2)
{
    uint32_t send_mail_box;
    shoot_tx_message.StdId = FRIC_ALL_ID;
    shoot_tx_message.IDE = CAN_ID_STD;
    shoot_tx_message.RTR = CAN_RTR_DATA;
    shoot_tx_message.DLC = 0x08;
    
   	shoot_can_send_data[0] = shoot1 >> 8;
    shoot_can_send_data[1] = shoot1;
		shoot_can_send_data[2] = shoot2 >> 8;
    shoot_can_send_data[3] = shoot2;

    HAL_CAN_AddTxMessage(&hcan2, &shoot_tx_message, shoot_can_send_data, &send_mail_box);
}

/**
* @brief Ò£¿ØÆ÷·¢ËÍ
* @param 
*/
void CAN_cmd_RC1(int16_t ch0,int16_t ch1,int16_t ch2,int16_t ch3)
{
    uint32_t send_mail_box;
    rc_tx_message.StdId = 0x567;
    rc_tx_message.IDE = CAN_ID_STD;
    rc_tx_message.RTR = CAN_RTR_DATA;
    rc_tx_message.DLC = 0x08;
    
    rc_can_send_data[0] = ch0 >> 8;
    rc_can_send_data[1] = ch0;
		rc_can_send_data[2] = ch1 >> 8;
		rc_can_send_data[3] = ch1;
		rc_can_send_data[4] = ch2 >> 8;
		rc_can_send_data[5] = ch2;
		rc_can_send_data[6] = ch3 >> 8;
		rc_can_send_data[7] = ch3;

    HAL_CAN_AddTxMessage(&hcan2, &rc_tx_message, rc_can_send_data, &send_mail_box);

}
/**
* @brief Ò£¿ØÆ÷·¢ËÍ
* @param 
*/
void CAN_cmd_RC2(uint8_t s1,uint8_t s2,int16_t sw,uint16_t key)
{
    uint32_t send_mail_box;
    rc_tx_message.StdId = 0x520;
    rc_tx_message.IDE = CAN_ID_STD;
    rc_tx_message.RTR = CAN_RTR_DATA;
    rc_tx_message.DLC = 0x06;
    
    rc_can_send_data[0] = s1;
    rc_can_send_data[1] = s2;
		rc_can_send_data[2] = sw >> 8;
		rc_can_send_data[3] = sw;
		rc_can_send_data[4] = key >> 8;
		rc_can_send_data[5] = key;

    HAL_CAN_AddTxMessage(&hcan2, &rc_tx_message, rc_can_send_data, &send_mail_box);

}
int Cacapacitance_Rong(void)
{
	  return super_capacitor.rong;
}
