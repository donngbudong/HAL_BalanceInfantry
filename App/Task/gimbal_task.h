#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H

#include "can_receive.h"
#include "System.h"


#define IMU_90          90
#define IMU_180         180
#define IMU_360         360

#define PITCH_MAX				25
#define PITCH_MIX				-20

/* 云台电机数组下标 */
typedef enum 
{
  GIM_YAW = 0,
  GIM_PITCH = 1,
  GIM_MOTOR_CNT = 2,
}GIM_Motor_cnt_t;




typedef struct
{
  float Angle_Inc;
  float Angle_k;//角度增加的幅度调节因子
  float Slow_Inc;
}Gimbal_Move_t;

typedef struct
{
  Motor_Info_t  Motor_Info;
  Motor_Data_t  Motor_Date;
  PID_Info_t    PID;
	
	Gimbal_Move_t RC_Move;
	Gimbal_Move_t	KEY_Move;
	
}Gimbal_Info_t;


typedef struct 
{
	System_State_t State;
	System_Ctrl_Mode_t Ctrl_Mode;
	Gimbal_Info_t YAW;
	Gimbal_Info_t PITCH;
  PID_Type_t PID_Type;
}Gimbal_Date_t;


void Gimbal_Task(void);

void Gimbal_GET_Info(void);
void Gimbal_Get_PID_Type(Gimbal_Info_t *strGim);
void Gimbal_Set_PID_type(void);
void Gimbal_InitPID(void);
void Gimbal_PID_Switch(Gimbal_Info_t *strGim);

void Gimbal_RC_Ctrl(void);

void RC_Ctrl_YAW(Gimbal_Info_t *str);
void RC_Ctrl_PITCH(Gimbal_Info_t *str);

float YAW_Angle_Over_Zero(float *Angle);
float PITCH_Angle_Limit(float *Angle);

float Gimbal_GetOutPut(Gimbal_Info_t *str);
void Gimbal_CanOutPut(void);
void Gimbal_Stop(void);

void Gimbal_FirstYawAngle(Gimbal_Info_t *str);
void Gimbal_FirstPitchAngle(Gimbal_Info_t *str);

void IMU_YawData_Report(Motor_Data_t *str);
void IMU_PitData_Report(Motor_Data_t *str);
void IMU_Data_Report(void);

///* 云台信息结构体 */
//typedef struct
//{
//	
//	

//}GIM_Info_t;



///* 云台信息结构体 */
//typedef struct
//{
////  System_Ctrl_Mode_t ctrl_mode;
////  System_Pid_Mode_t pid_mode;
////  System_Action_t action_mode;
////  PID_type_t PID_type;  
////  AXIS_Info_t YAW;
////  AXIS_Info_t PIT;
////  Calibrate_State_t GIM_Cal_state;
////  bool IF_NAN;
////  bool IF_CALIBRATE_OK;
////  bool IF_LRDenfense_START;
////  Turn_Info_t Turn_Info;
////  Defense_Mode_t Defense_Mode;
////  Car_HalfMove_type_t HalfMove_type;
//}GIM_Info_t;




///*遥控器灵敏度*/
//#define Remote_yaw_dB				0.001f
//#define Remote_pitch_dB			0.0001f

///*鼠标灵敏度*/
//#define PC_pitch_dB					0.0025f
//#define PC_yaw_dB						0.01f

//void gimbal_pid_init(void);
//void pitch_limit(float *a, float imu_MAX,float imu_MIX);
//float yaw_limit(float *a);
//void gimbal_rc_dbus(void);
//void gimbal_rc_pc(void);
//void yaw_pitch_gimbal(void);
//void visual_giambal(void);
//void giambal_mode(void);

//void gimbal_ctrl(void);
//void gimbal_pid(void);
extern Gimbal_Date_t Gimbal;
#endif

