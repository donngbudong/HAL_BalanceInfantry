#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H
/************************************** Includes **************************************/
//#include "can_receive.h"

#include "Device.h"
#include "system.h"
//#include "pid.h"


/********************************* Exported functions *********************************/
#define Diameter_weel 			0.200f

/* 底盘电机数组下标 */
typedef enum 
{
  MF9025_R = 0,			//左前
  MF9025_L = 1,			//右前
  CHAS_MOTOR_CNT = 2,
}CHAS_Motor_cnt_t;





/* 底盘结构体 */
typedef struct
{
  System_State_t State;
	System_Ctrl_Mode_t Ctrl_Mode;
	Motor_Info_t  Motor_Info;

	CAN_MF9025_DATE_T Motor_Date[CHAS_MOTOR_CNT];		//电机数据
	PID_Chassis_Info_t PID;													//PID
	PID_Type_t PID_Type;														//PID类型
	float torque_const;								//转矩常数

	float follow_gimbal_zero;												//跟随云台零点

	float speed_x,target_speed_x;										//底盘速度
	float pose_x,target_pose_x;											//底盘位移	
	float Pitch;																		//底盘倾角
	float Yaw;																			//底盘旋转偏角
	
	float Gyo_y;																		//底盘倾角角速度
//	float Yaw;																			//底盘旋转偏角由yaw轴6020确定
	float omega_z;																	//底盘旋转线速度

  float X_Speed_k ,Y_Speed_k,Z_Speed_k; 						//遥控->速度 变化的幅度调节因子
	float X_Target,Y_Target,Z_Target;								//目标速度
	
	float torque_speed;															//前进动力转矩
	float torque_balance;														//平衡动力转矩
	float torque_revolve;														//旋转动力转矩
	float iqControl[2];															//转矩电流输出


}CHASSIS_Date_t;




extern CHASSIS_Date_t Chassis;


void Chassis_Task(void);
void Chassis_GET_Info(void);
void Chassis_Balance(void);


void Chassis_Get_PID_Type(PID_Info_t *str);
void Chassis_Set_PID_Type(void);
void Chassis_InitPID(void);
void Chassis_Stop(void);
void Chassis_PID_Set(PID_Chassis_Info_t *str);
void Chassis_RC_Ctrl(void);

#endif
