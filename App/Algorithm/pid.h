#ifndef __PID_H_
#define __PID_H_
/************************************** Includes **************************************/
//#include "Device.h"

//#include "gimbal_task.h"
/********************************* Exported functions *********************************/

typedef enum
{
	Clear_Away  =0,	    //清除
	KEY         =1,			//键盘
	RC          =2,			//遥控
	PID_TYPE_CNT,
}PID_Type_t;


/* PID参数 */
typedef __packed struct 
{
  float P,I,D;
}PID_Parameter_t;


typedef __packed struct
{
  PID_Parameter_t PID_Param;
	float Target;
	float Actual;
  float Err;
  float Last_Err;
	
  float I_Limit;				//I积分限幅
  float I_Limit_Max;	
	
  float PID_P_Out;
  float PID_I_Out;
  float PID_D_Out;
  float PID_Output;
  float PID_Err_Dead;			//死区
  float PID_Output_Max;		//输出限幅
  float PID_I_Out_Max;		
  float PID_P_Out_Max;
}PID_Loop_t;

/*底盘PID数据结构*/
typedef struct 
{  
	PID_Loop_t PID_b1;
	PID_Loop_t PID_b2;
	
  PID_Loop_t PID_b3;
	PID_Loop_t PID_b4;
	
	PID_Loop_t PID_b5;
	PID_Loop_t PID_b6;
	
	PID_Loop_t Chasssis_OUT;
  PID_Type_t PID_Type;
}PID_Chassis_Info_t;

/*云台PID数据结构体*/
typedef struct 
{
  PID_Loop_t Speed_Loop;
	PID_Loop_t Angle_Loop;
  PID_Type_t PID_Type;
}PID_Info_t;


#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


float PID_Position(PID_Loop_t *pid, float target, float actual);

void Motor_Init(void);
void MF_9025_Init(void);
		
void MF_9028_Balance_Kp(PID_Loop_t *str);
void MF_9028_Balance_Kd(PID_Loop_t *str);
void MF_9028_PID_56(PID_Loop_t *str);
void MF_9028_Chasssis_OUT(PID_Loop_t *str);

void GM_6020_Init(void);
//void GIM_YawParamInit(Gimbal_Info_t *str);
//void GIM_PitParamInit(Gimbal_Info_t *str);


#endif
