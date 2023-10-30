#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H
#include "stm32f4xx.h"                  // Device header
#include "pid.h"
#include "HI229.h"


/* �������ö�� */
typedef enum 
{
  MF_9025_Right,
	MF_9025_Left,
  GM_6020_YAW ,
  GM_6020_PIT ,
  M_2006  ,
  FRIC_3508 ,
  MOTOR_TYPE_CNT,
}Motor_Type_t;

///*���̵��ID��Ϣ*/
//typedef enum
//{
//	CHASSIS_ALL_ID = 0x200,
//	CAN_3508_M1_ID = 0x201,
//	CAN_3508_M2_ID = 0x202,
//	CAN_3508_M3_ID = 0x203,
//	CAN_3508_M4_ID = 0x204,
//}CHASSIS_MOTOR_ID;


/*����9025���ID��Ϣ*/
typedef enum
{
	CAN_MF9025_ALL_ID = 0x280,
	CAN_MF9025_1_ID = 0x141,
	CAN_MF9025_2_ID = 0x142,
}CHASSIS_MOTOR_ID;
/* ��̨���ID��Ϣ */
typedef enum 
{
	GIMBAL_ALL_ID = 0x1FF,   //��̨���ID
  GIM_YAW_ID 	  = 0x205,   //YAW
  GIM_PIT_ID    = 0x206,   //PITCH
}GIMBAL_MOTOR_ID;


/* ���̵��ID��Ϣ */
typedef enum 
{
	DRIVER_ALL_ID = 0x1FF,
  DRIVER_ID     = 0x207,   
}DRIVER_MOTOR_ID;


/* Ħ���ֵ��ID��Ϣ */
typedef enum 
{
  FRIC_ALL_ID = 0x200,
  FRIC_L_ID   = 0x201,   
  FRIC_R_ID   = 0x202,
}FRIC_MOTOR_ID;


/*�����Ϣ�ṹ��*/
typedef struct 
{
  Motor_Type_t Motor_Type;
}Motor_Info_t;


/*CAN���յ������� �ṹ��*/
typedef __packed struct 
{
  int16_t Motor_Angle;
	int16_t Motor_Speed;
  int16_t Motor_ELC;
  uint8_t Motor_Temp;   
}CAN_GET_DATA_t;

/*MF9025���յ�������*/
typedef struct
{
	int8_t Temp;
	int16_t Speed;
	int16_t Torque_Current;
	uint16_t Encoder;
	uint16_t Last_Encoder;
}CAN_MF9025_DATE_T;

/*���IMU���յ������� �ṹ��*/
typedef __packed struct
{
  float IMU_Speed;
  float IMU_Angle;
}IMU_CAN_DATA_t;

typedef struct
{
	CAN_GET_DATA_t CAN_GetData;
	IMU_CAN_DATA_t IMU_GetData;
  /* ��������pid���� */
  float PID_Speed;
  float PID_Speed_target;
  float PID_Angle; 
  float PID_Angle_target; 
}Motor_Data_t;


//typedef struct
//{
//	CAN_MF9025_DATE_T CAN_GetData;
//}Motor_MF9025_t;

extern IMU_GET_DATA_t	 IMU_Get_Data;
extern PID_Parameter_t Balance_Kp_Param;
extern PID_Parameter_t Balance_Kd_Param;
extern PID_Parameter_t Chasssis_OUT;

extern PID_Parameter_t PID_Speed_Param[MOTOR_TYPE_CNT][PID_TYPE_CNT];
extern PID_Parameter_t PID_Angle_Param[MOTOR_TYPE_CNT][PID_TYPE_CNT];






/*�����������ݽṹ��*/
typedef struct
{
    int chi_voltage;
    int power;
    int rong_voltage;
    uint8_t state;
		int rong;
}super_capacitor_t;

void CAN_MF9025_Decode(CAN_MF9025_DATE_T *str,uint8_t *Date);


void get_motor_measure(CAN_GET_DATA_t *ptr, uint8_t *data);
void capacitance_date(super_capacitor_t *ptr, uint8_t *data);

void CAN_cmd_capacitance(int16_t size);
void CAN_cmd_chassis(int16_t Right, int16_t Left);
void CAN_cmd_gimbal_yaw(int16_t yaw);
void CAN_cmd_gimbal_pitch(int16_t pitch,int16_t driver);
void CAN_cmd_shoot( int16_t shoot1,int16_t shoot2);
void CAN_cmd_RC( int16_t s1,int16_t s2);

int Cacapacitance_Rong(void);


#endif

