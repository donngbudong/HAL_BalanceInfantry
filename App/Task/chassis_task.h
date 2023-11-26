#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H
/************************************** Includes **************************************/
//#include "can_receive.h"

#include "Device.h"
#include "system.h"
//#include "pid.h"


/********************************* Exported functions *********************************/
#define Diameter_weel 			0.200f

/* ���̵�������±� */
typedef enum 
{
  MF9025_R = 0,			//��ǰ
  MF9025_L = 1,			//��ǰ
  CHAS_MOTOR_CNT = 2,
}CHAS_Motor_cnt_t;





/* ���̽ṹ�� */
typedef struct
{
  System_State_t State;
	System_Ctrl_Mode_t Ctrl_Mode;
	Motor_Info_t  Motor_Info;

	CAN_MF9025_DATE_T Motor_Date[CHAS_MOTOR_CNT];		//�������
	PID_Chassis_Info_t PID;													//PID
	PID_Type_t PID_Type;														//PID����
	float torque_const;								//ת�س���

	float follow_gimbal_zero;												//������̨���

	float speed_x,target_speed_x;										//�����ٶ�
	float pose_x,target_pose_x;											//����λ��	
	float Pitch;																		//�������
	float Yaw;																			//������תƫ��
	
	float Gyo_y;																		//������ǽ��ٶ�
//	float Yaw;																			//������תƫ����yaw��6020ȷ��
	float omega_z;																	//������ת���ٶ�

  float X_Speed_k ,Y_Speed_k,Z_Speed_k; 						//ң��->�ٶ� �仯�ķ��ȵ�������
	float X_Target,Y_Target,Z_Target;								//Ŀ���ٶ�
	
	float torque_speed;															//ǰ������ת��
	float torque_balance;														//ƽ�⶯��ת��
	float torque_revolve;														//��ת����ת��
	float iqControl[2];															//ת�ص������


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
