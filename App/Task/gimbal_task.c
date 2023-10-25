/***      
�� ��̨���ƴ��� ��
***/
#include "gimbal_task.h"
#include "Device.h"



/* Init start */
Gimbal_Date_t Gimbal ={
	.State = SYSTEM_LOST,
	.Ctrl_Mode = Ctrl_Err,
	.PID_Type = RC,		//Clear_Away
};


/**
 * @brief ��̨�ܿ�
 * @param 
 */
void Gimbal_Task(void)
{
	Gimbal_GET_Info();
	
	if(RC_S1==1)
	{
		Gimbal_RC_Ctrl();
	}
	else
	Gimbal_Stop();
}


/**
 * @brief ��̨��Ϣ��ȡ
 * @param 
 */
void Gimbal_GET_Info(void)
{
	Gimbal.State = System.System_State;
	Gimbal_Set_PID_type();
	Gimbal_InitPID();
	IMU_Data_Report();
}



/**
 * @brief PID�����ȡ
 * @note  ����ͬ��
 */
void Gimbal_Get_PID_Type(Gimbal_Info_t *strGim)
{
  strGim->PID.PID_Type = Gimbal.PID_Type;
}

void Gimbal_Set_PID_type(void)
{
  Gimbal_Get_PID_Type(&Gimbal.YAW);
  Gimbal_Get_PID_Type(&Gimbal.PITCH);    
}

/**
 * @brief ��̨PID�����ܺ���
 * @param 
 */
void Gimbal_InitPID(void)
{  
  Gimbal_PID_Switch(&Gimbal.YAW);
  Gimbal_PID_Switch(&Gimbal.PITCH); 
}



/**
 * @brief ��̨PID�л�
 * @param 
 */
void Gimbal_PID_Switch(Gimbal_Info_t *strGim)
{
	PID_Info_t *str = &(strGim->PID);
	Motor_Info_t *strx = &(strGim->Motor_Info);

	static int length = sizeof(PID_Parameter_t);
 switch (strx->Motor_Type)
 {
	case GM_6020_YAW:{
		switch (str->PID_Type)
      {
        case RC:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_YAW][RC],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_YAW][RC],length);
        }break;
        case KEY:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_YAW][KEY],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_YAW][KEY],length);
        }break;
         default:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_YAW][Clear_Away],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_YAW][Clear_Away],length);
        }break;
			}
	}
	 case GM_6020_PIT:{
		 	switch (str->PID_Type)
      {
        case RC:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_PIT][RC],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_PIT][RC],length);
        }break;
        case KEY:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_PIT][KEY],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_PIT][KEY],length);
        }break;
         default:{
          memcpy(&(str->Speed_Loop.PID_Param),&PID_Speed_Param[GM_6020_PIT][Clear_Away],length);
          memcpy(&(str->Angle_Loop.PID_Param),&PID_Angle_Param[GM_6020_PIT][Clear_Away],length);
        }break;
			}
   }	 
	}
}




/**
 * @brief ң����������̨
 * @param 
 */
void Gimbal_RC_Ctrl(void)
{
//	//��һ�ν���RCģʽ
//	Gimbal_FirstYawAngle(&Gimbal.YAW);
//	Gimbal_FirstYawAngle(&Gimbal.PITCH);
	
	RC_Ctrl_YAW(&Gimbal.YAW);
	RC_Ctrl_PITCH(&Gimbal.PITCH);
	
	Gimbal_CanOutPut();
}

/**
 * @brief Gimbal_YAW��Ŀ��� ң����ң��ģʽ
 * @param 
 */
void RC_Ctrl_YAW(Gimbal_Info_t *str)
{
	str->RC_Move.Angle_Inc = str->RC_Move.Angle_k * (-RC_CH0);
	str->Motor_Date.PID_Angle_target += str->RC_Move.Angle_Inc;
	YAW_Angle_Over_Zero(&str->Motor_Date.PID_Angle_target);
	
	/*ͬ����������YAW����*/
	str->Motor_Date.PID_Speed = str->Motor_Date.IMU_GetData.IMU_Speed;
	str->Motor_Date.PID_Angle = str->Motor_Date.IMU_GetData.IMU_Angle;
}
/**
 * @brief Gimbal_PITCH��Ŀ��� ң����ң��ģʽ
 * @param 
 */
void RC_Ctrl_PITCH(Gimbal_Info_t *str)
{
	str->RC_Move.Angle_Inc = str->RC_Move.Angle_k * RC_CH1;
//	str->Motor_Date.PID_Angle_target += str->RC_Move.Angle_Inc;
	str->Motor_Date.PID_Angle_target += str->RC_Move.Angle_Inc;
	PITCH_Angle_Limit(&str->Motor_Date.PID_Angle_target);
	
	/*ͬ����������YAW����*/
	str->Motor_Date.PID_Speed = str->Motor_Date.IMU_GetData.IMU_Speed;
	str->Motor_Date.PID_Angle = str->Motor_Date.IMU_GetData.IMU_Angle;
}
/**
 * @brief YAW��Ƕȹ��㴦��
 * @param 
 */
float YAW_Angle_Over_Zero(float *Angle)
{
	if(*Angle - Gimbal.YAW.Motor_Date.IMU_GetData.IMU_Angle > IMU_180)    
	{
		*Angle =*Angle - IMU_360;        
	}
	else if(*Angle - Gimbal.YAW.Motor_Date.IMU_GetData.IMU_Angle < -IMU_180)
	{
		*Angle =*Angle + IMU_360;
	}
	return *Angle;
}


/**
 * @brief PICTH��Ƕ���λ
 * @param 
 */
float PITCH_Angle_Limit(float *Angle)
{
	if(*Angle > PITCH_MAX)    
	{
		*Angle = PITCH_MAX;        
	}
	if(*Angle < PITCH_MIX)
	{
		*Angle =PITCH_MIX;
	}
	return *Angle;
}





/**
 * @brief ��ȡ����PID���
 * @param 
 */
float Gimbal_GetOutPut(Gimbal_Info_t *str)
{
  PID_Info_t *PID = &(str->PID);
  Motor_Data_t *DATA = &(str->Motor_Date);
  float res;
   
  /*�ٶȻ����� = �ǶȻ�PID���*/
  DATA->PID_Speed_target = PID_Position(&PID->Angle_Loop,
																				str->Motor_Date.PID_Angle_target ,\
																				str->Motor_Date.PID_Angle); \
  /*����ֵ = �ٶȻ�PID���*/
  res = PID_Position(&PID->Speed_Loop,
										str->Motor_Date.PID_Speed_target,\
										str->Motor_Date.PID_Speed);\
  return res;
}



/**
 * @brief Gimbal������
 * @param 
 */
void Gimbal_CanOutPut(void)
{
	static int16_t pid_out[4] = {0,0,0,0};
	
	pid_out[GIM_YAW]	= Gimbal_GetOutPut(&Gimbal.YAW);
	pid_out[GIM_PITCH]= Gimbal_GetOutPut(&Gimbal.PITCH);
	
	CAN_cmd_gimbal_yaw(pid_out[GIM_YAW]);
	CAN_cmd_gimbal_pitch(pid_out[GIM_PITCH],0);
}








/**
 * @brief ж������
 * @param 
 */
void Gimbal_Stop(void)
{
	static int16_t pid_out[2] = {0, 0};
	
	/* �ٶȻ�������� */
  pid_out[GIM_YAW] 	 = 0;
  pid_out[GIM_PITCH] = 0;
  
	CAN_cmd_gimbal_yaw(pid_out[GIM_YAW]);
	CAN_cmd_gimbal_pitch(pid_out[GIM_PITCH],0);
}
/**
 * @brief ��ʼ��������ģʽYAW��Ƕ�
 * @param 
 */
void Gimbal_FirstYawAngle(Gimbal_Info_t *str)
{
  str->Motor_Date.PID_Angle_target = str->Motor_Date.IMU_GetData.IMU_Angle;
}

/**
 * @brief ��ʼ��������ģʽPITCH��Ƕ�
 * @param 
 */
void Gimbal_FirstPitchAngle(Gimbal_Info_t *str)
{
  str->Motor_Date.PID_Angle_target = str->Motor_Date.IMU_GetData.IMU_Angle;
}


/**
 * @brief ��ȡIMU����
 * @param 
 */
void IMU_YawData_Report(Motor_Data_t *str)
{
  str->IMU_GetData.IMU_Speed = IMU_Get_Data.IMU_Gyo[Z]/10;
  str->IMU_GetData.IMU_Angle = IMU_Get_Data.IMU_Eular[YAW];
}
void IMU_PitData_Report(Motor_Data_t *str)
{
  str->IMU_GetData.IMU_Speed = IMU_Get_Data.IMU_Gyo[Y]/10;
  str->IMU_GetData.IMU_Angle = IMU_Get_Data.IMU_Eular[PITCH];
}
void IMU_Data_Report(void)
{
  IMU_YawData_Report(&Gimbal.YAW.Motor_Date);
  IMU_PitData_Report(&Gimbal.PITCH.Motor_Date);
}

//void gimbal_pid_init()
//{
//	PID_init(&pid_gimbal[1],15 ,	0	,		0,10000,5000);
//	PID_init(&pid_gimbal[2],160,	0.5,	0,28000,10000);
//	
//	PID_init(&pid_gimbal[3],10 	, 0 ,	 0,10000,5000);//8
//	PID_init(&pid_gimbal[4],150	,	2  	,0,28000,10000);//60
//}


//void pitch_limit(float *a, float imu_MAX,float imu_MIX)
//{
//    if(*a > imu_MAX)
//        *a = imu_MAX;
//	
//    if(*a < imu_MIX)
//        *a = imu_MIX;
//}

//float yaw_limit(float *a)
//{
//	if(*a-Eular[2] >180)    
//	{
//		*a =*a-360;        
//	}
//	else if(*a-Eular[2] <-180)
//	{
//		*a =*a+ 360;
//	}
//		return *a;
//}



//float yaw=0,pitach=0;
//float visual_yaw=0,visual_pitch=0;

//void gimbal_rc_dbus(void)
//{
//	yaw=yaw-RC_CH0*Remote_yaw_dB;
//	pitach=pitach+RC_CH1*Remote_pitch_dB;
//}


//void gimbal_rc_pc(void)
//{
//	yaw=yaw-MOUSE_X_MOVE_SPEED*PC_yaw_dB;
//	pitach=pitach-MOUSE_Y_MOVE_SPEED*PC_pitch_dB;
//}


//void yaw_pitch_gimbal(void)
//{
//	if(Yaw_Start == true) //������,Yaw�������нǶ�
//	{		
//		yaw=Eular[2];
//		Yaw_Start = false;
//	}
//	yaw_limit(&yaw);
//	PID_calc(&pid_gimbal[1],Eular[2],yaw);
//	PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);
//	//Pitch
//	pitch_limit(&pitach,25,-23);
//	PID_calc(&pid_gimbal[3],Eular[0],pitach);//pitach
//	PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);
//}

//void visual_giambal(void)
//{		
//		
//	visual_yaw=Eular[2] - Visual_Data.Vision_Yaw*0.7f;
//	yaw_limit(&visual_yaw);	
//	PID_calc(&pid_gimbal[1],Eular[2],visual_yaw);
//	PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);
//Pitch
//	visual_pitch=Eular[0] + Visual_Data.Vision_Pitch*0.5f;
//	pitch_limit(&visual_pitch,25,-23);
//	PID_calc(&pid_gimbal[3],Eular[0],visual_pitch);
//	PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);
//	 
//}


//void giambal_mode(void)	
//{
//	if(rc_ctrl.mouse.press_r==0)
//	{
//		yaw_pitch_gimbal();
//	}
//	else
//	{
//		if(Visual_Date_t==1)
//		{
//			visual_giambal();
//			yaw=Eular[2];
//			pitach=Eular[0];
//		}
//		else
//		{
//			yaw_pitch_gimbal();
//		}
//	}
//}

//	

//float driver_out; 
//extern uint16_t shoot_1;//����mode1����   0����
//void gimbal_ctrl(void)
//{

//	if(rc_ctrl.rc.s1==1)
//	{	
//		yaw_pitch_gimbal();
//		gimbal_rc_dbus();
//		CAN_cmd_gimbal_pitch(pid_gimbal[4].out,pid_shoot[5].out);
//		CAN_cmd_gimbal_yaw(pid_gimbal[2].out);
//	}
//	
//	if(rc_ctrl.rc.s1==3)
//	{	
//		CAN_cmd_gimbal_pitch(0,0);
//		CAN_cmd_gimbal_yaw(0);
//	}
//	
//	if(rc_ctrl.rc.s1==2)//pc
//	{	
//		gimbal_rc_pc();
//		giambal_mode();//�Ӿ��л�
//		if(shoot_1==0)
//		{
//			CAN_cmd_gimbal_pitch(pid_gimbal[4].out,pid_shoot[5].out);
//		}
//		else if(shoot_1==1)
//		{
//			CAN_cmd_gimbal_pitch(pid_gimbal[4].out,pid_shoot[4].out);
//		}
//		CAN_cmd_gimbal_yaw(pid_gimbal[2].out);
//	}
//}








//void gimbal_ctrl(void)
//{
//	if(rc_ctrl.rc.s1==1)
//	{

//	if(rc_ctrl.rc.s2==1)
//	{	//Yaw
////		PID_calc(&pid_gimbal[1],Eular[2],20);
////		PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);
//		//Pitch
//		PID_calc(&pid_gimbal[3],Eular[0],-10);
//		PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);

//	}
//	
//		else if(rc_ctrl.rc.s2==3)
//	{
////		PID_calc(&pid_gimbal[1],Eular[2],0);
////		PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);
//	
//		PID_calc(&pid_gimbal[3],Eular[0],0);
//		PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);
//	}
//	
//		else if(rc_ctrl.rc.s2==2)
//	{
////		PID_calc(&pid_gimbal[1],Eular[2],-20);
////		PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);

//		PID_calc(&pid_gimbal[3],Eular[0],10);
//		PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);
//	}
////	
//		PID_calc(&pid_gimbal[1],Eular[2],0);
//		PID_calc(&pid_gimbal[2],Gyo[2]/10,pid_gimbal[1].out);
//	
////		PID_calc(&pid_gimbal[3],Eular[0],0);
////		PID_calc(&pid_gimbal[4],Gyo[1]/10,pid_gimbal[3].out);	
////	
//		CAN_cmd_gimbal_pitch(pid_gimbal[4].out,0);
//		CAN_cmd_gimbal_yaw(pid_gimbal[2].out);

//}
//		if(rc_ctrl.rc.s1==3)
//		{
//					CAN_cmd_gimbal_pitch(0,0);
//					CAN_cmd_gimbal_yaw(0);
//		}
//}

