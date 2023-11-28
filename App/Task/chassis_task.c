/***      
《 底盘控制代码 》
***/
#include "chassis_task.h"
#include "Device.h"


/* Init start */
CHASSIS_Date_t Chassis ={
	.State=SYSTEM_LOST,
	.Ctrl_Mode = Ctrl_Err,
	.PID_Type = RC,
};




/**
 * @brief 底盘总控
 * @param 
 */
void Chassis_Task(void)
{
	Chassis_GET_Info();
//	CAN_cmd_RC1(RC_CH0,RC_CH1,RC_CH2,RC_CH3);
//	CAN_cmd_RC2(RC_S1,RC_S2,RC_SW,RC_Ctrl.kb.key);
	if(RC_S1==1)
	{
		Chassis_Balance();
		CAN_cmd_chassis(Chassis.iqControl[0],Chassis.iqControl[1]);
	}
	else{
		Chassis_Stop();
	}
}



/**
* @brief 底盘信息获取
* @param 
*/
void Chassis_GET_Info(void)
{
	Chassis.State = System.System_State;
	Chassis.Pitch = IMU_Get_Data.IMU_Eular[1];
	Chassis.Gyo_y		= IMU_Get_Data.IMU_Gyo[0]/10;//除10
	Chassis.speed_x = -(Chassis.Motor_Date[0].Speed - Chassis.Motor_Date[1].Speed) /57.29578f*Diameter_weel/2.0f/2.0f ;
	Chassis.omega_z =  (Chassis.Motor_Date[0].Speed + Chassis.Motor_Date[1].Speed) /57.29578f*Diameter_weel/2.0f/2.0f;
	Chassis.pose_x += 	Chassis.speed_x * 0.001f;
	Chassis.X_Target = 	RC_CH3;
	Chassis.Y_Target =	RC_CH2;
	Chassis.Z_Target =  RC_CH0;

}


void Chassis_Balance(void)
{
	PID_Position(&Chassis.PID.PID_b3,-4,Chassis.Pitch);
	PID_Position(&Chassis.PID.PID_b4,0,Chassis.Gyo_y);
	
	Chassis.torque_balance = Chassis.PID.PID_b3.PID_Output+Chassis.PID.PID_b4.PID_Output;
	
	PID_Position(&Chassis.PID.Chasssis_OUT,Chassis.torque_balance,0);
	Chassis.iqControl[0] = -Chassis.PID.Chasssis_OUT.PID_Output;
	Chassis.iqControl[1] =  Chassis.PID.Chasssis_OUT.PID_Output;

}





/**
* @brief 底盘PID设置总函数
* @param 
*/
void Chassis_InitPID(void)
{
	Chassis_PID_Set(&Chassis.PID);
}


/**
 * @brief PID初始化
 * @param 
 */
void Chassis_PID_Set(PID_Chassis_Info_t *str)
{
  static int length = sizeof(PID_Parameter_t);
	memcpy(&(str->PID_b3),&Balance_Kp_Param,length);
	memcpy(&(str->PID_b4),&Balance_Kd_Param,length);
	memcpy(&(str->Chasssis_OUT),&Chasssis_OUT,length);
};



/**
 * @brief 卸力函数
 * @param 
 */
void Chassis_Stop(void)
{
	static int16_t pid_out[CHAS_MOTOR_CNT] = {0,0};
	
	/* 速度环最终输出 */
  pid_out[MF9025_R] = 0;
  pid_out[MF9025_L] = 0;
	CAN_cmd_chassis(pid_out[MF9025_R],pid_out[MF9025_L]);	  
}








/*----------------------------------结构体-----------------------------*/
//extern pid_type_def pid_chassis[8];
//extern motor_measure_t motor_chassis[5];
//extern motor_measure_t motor_gimbal[3];
//extern RC_ctrl_t rc_ctrl;
//rc_3508 rc_3508_t;

//uint16_t Power_Shift=0;

//uint16_t YAW_MotorAngle_Proc(int16_t Angle)
//{
//	if(YAW_ZERO_DEGREE - Angle>4096)
//	{
//		Angle+=8192;
//	}
//	else if(YAW_ZERO_DEGREE - Angle<-4096)
//	{
//		Angle-=8192;
//	}
//	
//  return Angle;
//}


//void xy_speed(int32_t *speed, float MAX,float MIX)
//{
//	if(*speed > MAX){
//		*speed = MAX;
//	}
//	if(*speed < MIX){ 
//	*speed = MIX;
//	}
//}




//void chassis_to_common(void)
//{	
//	rc_3508_t.wz_set = 0;
//	rc_3508_t.wheel_speed[0] =  rc_3508_t.vx_set - rc_3508_t.vy_set + rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[1] = -rc_3508_t.vx_set - rc_3508_t.vy_set + rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[2] = -rc_3508_t.vx_set + rc_3508_t.vy_set + rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[3] =  rc_3508_t.vx_set + rc_3508_t.vy_set + rc_3508_t.wz_set;
//}

//static float sin_yaw_x = 0.0f;
//static float cos_yaw_y = 0.0f;	
//void chassis_to_gyro(void)
//{	
//	static float Yaw_Angle_t = 0;
//	static float Yaw_Angle;
//	
//	Yaw_Angle=YAW_ZERO_DEGREE - motor_gimbal[1].ecd;
//	if(Yaw_Angle < 0)
//	{
//		Yaw_Angle += 8192;
//	}
//	Yaw_Angle_t=Yaw_Angle*2*3.1415926f/8192;
//	
//	CHAS_gyro();//小陀螺
//	sin_yaw_x= cos(Yaw_Angle_t) * rc_3508_t.vx_set - sin(Yaw_Angle_t) * rc_3508_t.vy_set;
//	cos_yaw_y= sin(Yaw_Angle_t) * rc_3508_t.vx_set + cos(Yaw_Angle_t) * rc_3508_t.vy_set;
//	rc_3508_t.wheel_speed[0] =  sin_yaw_x - cos_yaw_y  + rc_3508_t.wz_set;//4
//	rc_3508_t.wheel_speed[1] = -sin_yaw_x - cos_yaw_y  + rc_3508_t.wz_set;//3
//	rc_3508_t.wheel_speed[2] = -sin_yaw_x + cos_yaw_y  + rc_3508_t.wz_set;//2
//	rc_3508_t.wheel_speed[3] =  sin_yaw_x + cos_yaw_y  + rc_3508_t.wz_set;//1
//}

//void CHAS_gyro(void)
//{
//	 switch((int)REF_FOOT_CHAS_MaxPower())
//  {
//    case 45:/**血量优先1级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 2500	;
//			}
//			else{
//				rc_3508_t.wz_set = 3000;
//			}
//        break;
//    case 50:/**血量优先2级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 2700;
//			}
//			else{
//				rc_3508_t.wz_set = 3200;
//			}
//        break;
//    case 55:/**血量优先3级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 3000;
//			}
//			else{
//				rc_3508_t.wz_set = 3500;
//			}

//        break;
//    case 60:/**功率优先1级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 4000;
//			}
//			else{
//				rc_3508_t.wz_set = 4500;
//			}

//        break;
//    case 80:/**功率优先2级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 4500;
//			}
//			else{
//				rc_3508_t.wz_set = 5000;
//			}
//        break;
//    case 100:/**功率优先3级**/
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 5000;
//			}
//			else{
//				rc_3508_t.wz_set = 5500;
//			}
//        break;
//    default:
//			if(sin_yaw_x!=0||cos_yaw_y!=0){
//				rc_3508_t.wz_set = 3500;
//			}
//			else{
//				rc_3508_t.wz_set = 3000;
//			}
//        break;
//	}
//}

////底盘跟随
//int16_t yaw_angle_t;
//void chassis_yaw(void)
//{	
//	yaw_angle_t=YAW_MotorAngle_Proc(motor_gimbal[1].ecd);
//	PID_calc(&pid_chassis[5],yaw_angle_t,YAW_ZERO_DEGREE);
//	PID_calc(&pid_chassis[6],motor_gimbal[1].speed_rpm,pid_chassis[5].out);
// 
//	if(pid_chassis[5].error[0] > -50 && pid_chassis[5].error[0] < 50){
//			rc_3508_t.wz_set = 0;
//		}
//	else {
//			rc_3508_t.wz_set = pid_chassis[5].out;
//		}
//	rc_3508_t.wheel_speed[0] =  rc_3508_t.vx_set - rc_3508_t.vy_set +  rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[1] = -rc_3508_t.vx_set - rc_3508_t.vy_set +  rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[2] = -rc_3508_t.vx_set + rc_3508_t.vy_set +  rc_3508_t.wz_set;
//	rc_3508_t.wheel_speed[3] =  rc_3508_t.vx_set + rc_3508_t.vy_set +  rc_3508_t.wz_set;

//}

//void chassis_rc_dbus(void)
//{			
//	rc_3508_t.vx_set = rc_ctrl.rc.ch3 * 4000/660;
//	rc_3508_t.vy_set = rc_ctrl.rc.ch2 * 4000/660;
//	if(rc_ctrl.rc.s1 == 1)
//	{
//		
//		if(rc_ctrl.rc.s2==1)
//		{
//			chassis_to_common();
//		}
//		
//		else if(rc_ctrl.rc.s2==2)
//		{					
//			chassis_to_gyro();
//		}		
//		
//		else if(rc_ctrl.rc.s2==3)
//		{
//			chassis_yaw();
//		}
//	}
//}

////pc
//uint16_t Z=1, X, C;//z:底盘跟随  x:普通模式 c:小陀螺
//void chassis_rc_pc(void)
//{
//	CHAS_MaxPower();

//	if(rc_ctrl.key.v & KEY_PRESSED_OFFSET_W){
//		rc_3508_t.vx_set+=20;
//	}
//	
//	else if(rc_ctrl.key.v & KEY_PRESSED_OFFSET_S){
//		rc_3508_t.vx_set-=20;
//	}
//	
//	else if(rc_ctrl.key.v & KEY_PRESSED_OFFSET_A){
//		rc_3508_t.vy_set-=20;
//	}
//	
//	else if(rc_ctrl.key.v & KEY_PRESSED_OFFSET_D){
//		rc_3508_t.vy_set+=20;
//	}
//	else
//	{
//		rc_3508_t.vx_set = 0;
//  	rc_3508_t.vy_set = 0;
//	}
//	
//	Chassis_pc_Z();
//	Chassis_pc_X();
//	Chassis_pc_C();
//	if(Z == 1){
//		chassis_to_common();
//	}
//	if(C == 1){
//		chassis_to_gyro();
//	}
//	if(X == 1){
//		chassis_yaw();
//	}
//}




//void Chassis_pc_Z(void)
//{
//	static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_Z)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_Z)
//			{	
//				press_flag=0;
//				if(Z==0)
//				{
//					Z=1;
//					C=0;
//					X=0;
//				}
//				else	
//				{Z=0;}
//			}
//		}
//		if(press_flag==0&&Z==0)
//		{
//			press_flag=1;
//		}
//}

//void Chassis_pc_X(void)
//{
//	static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_X)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_X)
//			{	
//				press_flag=0;
//				if(X==0)
//				{
//					Z=0;
//					C=0;
//					X=1;}
//				else	
//				{X=0;}
//			}
//		}
//		if(press_flag==0&&X==0)
//		{
//			press_flag=1;
//		}
//}

//void Chassis_pc_C(void)
//{
//	static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_C)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_C)
//			{	
//				press_flag=0;
//				if(C==0){
//					Z=0;
//					C=1;
//					X=0;
//				}
//				else{
//				C=0;}
//			}
//		}
//		if(press_flag==0 && C==0)
//		{
//			press_flag=1;
//		}
//}
////void Chassis_pc_Shift(void)
////{
////	static uint8_t press_flag=1;
////	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_SHIFT)
////		{	
////			HAL_Delay(20);		
////			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_SHIFT)
////			{	
////				press_flag=0;
////				if(Power_Shift==0){
////					Power_Shift=10;
////				}
////				else{
////				C=0;}
////			}
////		}
////		if(press_flag==0 && C==0)
////		{
////			press_flag=1;
////		}
////}


//void CHAS_MaxPower(void)
//{
//  switch((int)REF_FOOT_CHAS_MaxPower())
//  {
//    case 45:
//				xy_speed(&rc_3508_t.vx_set,4000,-4000);
//				xy_speed(&rc_3508_t.vy_set,4000,-4000);
//				CAN_cmd_capacitance(43);
//        break;
//    case 50:
//				xy_speed(&rc_3508_t.vx_set,4500,-4500);
//				xy_speed(&rc_3508_t.vy_set,4500,-4500);
//				CAN_cmd_capacitance(48);
//        break;
//    case 55:
//				xy_speed(&rc_3508_t.vx_set,4500,-4500);
//				xy_speed(&rc_3508_t.vy_set,4500,-4500);
//				CAN_cmd_capacitance(53);

//        break;
//    case 60:
//				xy_speed(&rc_3508_t.vx_set,4500,-4500);
//				xy_speed(&rc_3508_t.vy_set,4500,-4500);
//				CAN_cmd_capacitance(59);

//        break;
//    case 80:
//				xy_speed(&rc_3508_t.vx_set,5500,-5500);
//				xy_speed(&rc_3508_t.vy_set,5500,-5500);
//				CAN_cmd_capacitance(79);
//        break;
//    case 100:
//				xy_speed(&rc_3508_t.vx_set,6000,-6000);
//				xy_speed(&rc_3508_t.vy_set,6000,-6000);
//				CAN_cmd_capacitance(99);

//        break;
//   
//    default:
//				xy_speed(&rc_3508_t.vx_set,4000,-4000);
//				xy_speed(&rc_3508_t.vy_set,4000,-4000);
//				CAN_cmd_capacitance(40);
//        break;
//	}
//}


//void chassis_ctrl(void)
//{
//	PID_calc(&pid_chassis[1],motor_chassis[1].speed_rpm,rc_3508_t.wheel_speed[0]);
//	PID_calc(&pid_chassis[2],motor_chassis[2].speed_rpm,rc_3508_t.wheel_speed[1]);
//	PID_calc(&pid_chassis[3],motor_chassis[3].speed_rpm,rc_3508_t.wheel_speed[2]);
//	PID_calc(&pid_chassis[4],motor_chassis[4].speed_rpm,rc_3508_t.wheel_speed[3]);
//	if(rc_ctrl.rc.s1 == 1)
//	{
//		chassis_rc_dbus();
//		CAN_cmd_chassis(pid_chassis[1].out,pid_chassis[2].out,pid_chassis[3].out,pid_chassis[4].out);
//	}

//	if(rc_ctrl.rc.s1 == 3)
//	{
//		CAN_cmd_chassis(0,0,0,0);
//	}

//	if(rc_ctrl.rc.s1 == 2)//pc
//	{
//		chassis_rc_pc();
//		CAN_cmd_chassis(pid_chassis[1].out,pid_chassis[2].out,pid_chassis[3].out,pid_chassis[4].out);
//	}
//}


////void chassis_ctrl(void)
////{	
////		
////	PID_calc(&pid_chassis[1],motor_chassis[1].speed_rpm,4000);
////	PID_calc(&pid_chassis[2],motor_chassis[2].speed_rpm,4000);
////	PID_calc(&pid_chassis[3],motor_chassis[3].speed_rpm,4000);
////	PID_calc(&pid_chassis[4],motor_chassis[4].speed_rpm,4000);
////	if (rc_ctrl.rc.s1 == 3)
////	{
////		CAN_cmd_chassis(0,0,0,0);
////	}
////	if (rc_ctrl.rc.s1 == 1)
////	{
////		CAN_cmd_chassis(pid_chassis[1].out,0,0,0);
////	}

////	if(rc_ctrl.rc.s1==2)
////	{
////		CAN_cmd_chassis(0,pid_chassis[2].out,0,0);
////	}
////	if (rc_ctrl.rc.s2 == 1)
////	{
////		CAN_cmd_chassis(0,0,pid_chassis[3].out,0);
////	}
////	if(rc_ctrl.rc.s2==2)
////	{
////		CAN_cmd_chassis(0,0,0,pid_chassis[4].out);
////	}
////}





