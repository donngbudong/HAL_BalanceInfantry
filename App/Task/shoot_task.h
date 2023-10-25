#ifndef __SHOOT_TASK_H
#define __SHOOT_TASK_H

#include "can_receive.h"
#include "System.h"


//#define ECD_FULL_ROUND 8192
//typedef uint8_t 	u8;
//typedef uint16_t 	u16;
//typedef uint32_t 	u32;



////拨盘电机模式枚举类型
//typedef enum
//{
//	DIAL_ONE=1,
//	DIAL_LONG=2,
//}Dial_Mode;



///*接收到的云台电机的参数结构体*/
//typedef struct{
//	uint16_t   real_angle;
//	uint16_t   pos_old;
//	uint16_t   pos;
//	float  	angle_err;				//abs angle range:[0,8191]
//	float  	angle_err_err;
//  int32_t  POS_ABS;
//	float		POS_angle;
//	
//	uint16_t 	angle;				//abs angle range:[0,8191]
//	uint16_t 	last_angle;	//abs angle range:[0,8191]
//	uint16_t	offset_angle;
//	int32_t		round_cnt;
//	int32_t	total_angle;
//	int32_t   ref_pos;
//}moto_measure_t;


/* 拨盘电机数组下标 */
typedef enum 
{
  FRIC_L = 0,//左
	FRIC_R = 1,
	DRIVER = 2,
  RIFLE_MOTOR_CNT = 3,
}SHOOT_Motor_cnt_t;

typedef struct
{
  Motor_Info_t Motor_Info;
  Motor_Data_t Motor_Data;
  PID_Info_t PID;
}Shoot_Motor_t;


typedef struct
{
	System_State_t State;
	System_Ctrl_Mode_t Ctrl_Mode;
	Shoot_Motor_t Motor_Date[RIFLE_MOTOR_CNT];
	PID_Type_t PID_Type;
}Shoot_Date_t;

//void shoot_pid_init(void);

//void Motor_Angle_Cal(moto_measure_t *ptr);
//void Shoot_SpeedLimit(void);

//void shoot_one(void);
//void shoot_long(void);

//void shoot_pc_G(void);
//void shoot_pc_R(void);
//void shoot_pc_F(void);
//void shoot_pc_Q(void);

//void shoot_rc_pc(void);
//void Shoot_CoolingLimit(void);



//void shoot_init(void);
//void shoot_ctrl(void);
extern Shoot_Date_t Shoot;


#endif


