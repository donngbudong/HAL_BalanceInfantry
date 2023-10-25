/**
  * @file       remote_control.c/h
  * @brief      遥控器处理，遥控器是通过类似SBUS的协议传输，利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
  * @note       该任务是通过串口中断启动，不是freeRTOS任务
  */

#include "remote_control.h"
#include "Device.h"


//遥控器控制变量
RC_Ctrl_t RC_Ctrl;

/**
  * @brief          遥控器协议解析
  * @param[in]      sbus_buf: 原生数据指针
  * @param[out]     RC_ctrl: 遥控器数据指
  * @retval         none
  */
void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_Ctrl_t *RC_ctrl)
{
	if (sbus_buf == NULL || RC_ctrl == NULL)
	{
			return;
	}
	RC_ctrl->rc.ch0 = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        	//!< Channel 0
	RC_ctrl->rc.ch1 = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; 	//!< Channel 1
	RC_ctrl->rc.ch2 = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          	//!< Channel 2
											(sbus_buf[4] << 10)) &0x07ff;
	RC_ctrl->rc.ch3 = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; 	//!< Channel 3
	RC_ctrl->rc.s1 = ((sbus_buf[5] >> 4) & 0x0003);                 				//!< 左上角132
	RC_ctrl->rc.s2 = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                    //!< 右上角132
	RC_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X
	RC_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y
	RC_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z
	RC_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press 
	RC_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press
	RC_ctrl->kb.key = sbus_buf[14] | (sbus_buf[15] << 8);                   //!< KeyBoard value
	RC_ctrl->rc.sw = sbus_buf[16] | (sbus_buf[17] << 8);                 		//拨轮
	//减去1024，让中间值为0
	RC_ctrl->rc.ch0 -= RC_CH_VALUE_OFFSET;
	RC_ctrl->rc.ch1 -= RC_CH_VALUE_OFFSET;
	RC_ctrl->rc.ch2 -= RC_CH_VALUE_OFFSET;
	RC_ctrl->rc.ch3 -= RC_CH_VALUE_OFFSET;
	//防止数据零漂，设置正负8的死区
	if(RC_ctrl->rc.ch0  <= 8 && RC_ctrl->rc.ch0 >= -8)
		RC_ctrl->rc.ch0 = 0;
	if(RC_ctrl->rc.ch1  <= 8 && RC_ctrl->rc.ch1 >= -8)
		RC_ctrl->rc.ch1 = 0;
	if(RC_ctrl->rc.ch2  <= 8 && RC_ctrl->rc.ch2 >= -8)
		RC_ctrl->rc.ch2 = 0;
	if(RC_ctrl->rc.ch3  <= 8 && RC_ctrl->rc.ch3 >= -8)
		RC_ctrl->rc.ch3 = 0;
}




/**
 * @brief 遥控器数据状态上报
 * @param 
 */
void RC_State_Report(void)
{
  System.Rc_State = RC_Check();
}


/**
 * @brief 遥控器总自检
 * @param 返回状态编号
 */
Rc_State_t RC_Check(void)
{
  static Rc_State_t res;
  res = RC_NORMAL;
	if(IF_RC_DATAERR)
    res = RC_ERR;
  if(IF_RC_LOST)
    res = RC_LOST;
  return res;
}



/**
 * @brief 遥控器数据自检
 * @param 
 */
bool Judge_RC_DataErr(void)
{
  static bool res ;
 	 res = false;
  if( ((RC_S1 != 1)&&(RC_S1 != 2)&&(RC_S1 != 3)) || ((RC_S2 != 1)&&(RC_S2 != 2)&&(RC_S2 != 3)) )
    res = true;
  else if(abs(RC_CH0)>660 ||
          abs(RC_CH1)>660 || 
          abs(RC_CH2)>660 ||
          abs(RC_CH3)>660 )
    res = true;
	return res;
}

/**
 * @brief 遥控器连接自检
 * @param 
 */
bool Judge_RC_Lost(void)
{
  static bool res ;
  	res = false;
  if(micros() >= Remote_time)
    res = true;
  return res;
}
