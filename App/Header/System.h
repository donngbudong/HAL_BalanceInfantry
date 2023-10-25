#ifndef __SYSTEM_H
#define __SYSTEM_H




/* 系统本机状态枚举 */
typedef enum 
{
	SYSTEM_ERR		 = 0,   //错误模式
	SYSTEM_NORMAL	 = 1,   //系统正常模式
	SYSTEM_LOST		 = 2,   //模块失联模式
}System_State_t;


/*RC状态枚举*/
typedef enum
{
	RC_ERR		= 0, 
	RC_NORMAL   = 1, 
	RC_LOST     = 2, 
}Rc_State_t;

/* IMU状态枚举 */
typedef enum
{
	IMU_ERR			= 0,
	IMU_NORMAL		= 1,
	IMU_LOST		= 2,
}Imu_State_t;

/* 系统控制状态枚举 */
typedef enum
{
  Ctrl_Err 			= 0,
  RC_CTRL_MODE 	= 1,    //遥控控制模式
  KEY_CTRL_MODE = 2,    //键盘控制模式
  CTRL_MODE_CNT ,
}System_Ctrl_Mode_t;


// /* 系统PID模式枚举 */
// typedef enum
// {
//   	PID_Err = 0,
// 	RC = 1,            	 //遥控模式
// 	KEY = 2,             //键鼠模式
//   	PID_MODE_CNT ,
// }System_Pid_Mode_t;


/* 系统信息结构体 */
typedef struct 
{
	Rc_State_t 			Rc_State;
	Imu_State_t 		Imu_State;
	System_State_t 	System_State;
}Sys_Info_t;




#include "stm32f4xx.h"
#include "drv_usart.h"

#include <stdio.h>
#include "stdbool.h"
#include "crc.h"
#include "referee.h"


extern Sys_Info_t System;


extern uint64_t		Remote_time;
extern uint64_t		Imu_time ;
uint32_t micros(void);
void Time_Init(void);
void System_State(void);

#endif
