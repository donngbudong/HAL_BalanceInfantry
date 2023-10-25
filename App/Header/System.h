#ifndef __SYSTEM_H
#define __SYSTEM_H




/* ϵͳ����״̬ö�� */
typedef enum 
{
	SYSTEM_ERR		 = 0,   //����ģʽ
	SYSTEM_NORMAL	 = 1,   //ϵͳ����ģʽ
	SYSTEM_LOST		 = 2,   //ģ��ʧ��ģʽ
}System_State_t;


/*RC״̬ö��*/
typedef enum
{
	RC_ERR		= 0, 
	RC_NORMAL   = 1, 
	RC_LOST     = 2, 
}Rc_State_t;

/* IMU״̬ö�� */
typedef enum
{
	IMU_ERR			= 0,
	IMU_NORMAL		= 1,
	IMU_LOST		= 2,
}Imu_State_t;

/* ϵͳ����״̬ö�� */
typedef enum
{
  Ctrl_Err 			= 0,
  RC_CTRL_MODE 	= 1,    //ң�ؿ���ģʽ
  KEY_CTRL_MODE = 2,    //���̿���ģʽ
  CTRL_MODE_CNT ,
}System_Ctrl_Mode_t;


// /* ϵͳPIDģʽö�� */
// typedef enum
// {
//   	PID_Err = 0,
// 	RC = 1,            	 //ң��ģʽ
// 	KEY = 2,             //����ģʽ
//   	PID_MODE_CNT ,
// }System_Pid_Mode_t;


/* ϵͳ��Ϣ�ṹ�� */
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
