/***      
�� ����������� ��
***/
#include "shoot_task.h"

//extern motor_measure_t motor_shoot[5];
//extern pid_type_def pid_shoot[8];
//extern RC_ctrl_t rc_ctrl;
//moto_measure_t  moto_shoot[2];




Shoot_Date_t Shoot = {
	.State = SYSTEM_LOST,
	.Ctrl_Mode = Ctrl_Err,
	.PID_Type = RC,
};

//void shoot_pid_init()
//{	//Ħ����
//	PID_init(&pid_shoot[1],15,0,0.5,7500,2000);
//	PID_init(&pid_shoot[2],15,0,0.5,7500,200);
//	//����
//	PID_init(&pid_shoot[3],5, 0, 0, 5000,5000);//1.23 2000
//	PID_init(&pid_shoot[4],12 ,  0, 0, 5000,5000);//0.8  2000
//	//long
//	PID_init(&pid_shoot[5],12 ,0,0 ,8000,15000);
//}


//uint8_t cnt = 1;
//void Motor_Angle_Cal(moto_measure_t *ptr)
//{
//	ptr->real_angle= motor_shoot[3].ecd/8192.0f*360.0f;
//	float  res1, res2;
//	
//	if(cnt)
//	{
//		ptr->pos_old=ptr->real_angle;
//		cnt=0;
//	}	
//	
//	ptr->pos = ptr->real_angle ;
//	ptr->angle_err=ptr->pos- ptr->pos_old;
//	
//	if(ptr->angle_err>0) 	
//	{
//		res1=ptr->angle_err-360;//��ת���Լ�
//		res2=ptr->angle_err;
//	}
//	else
//	{
//		res1=ptr->angle_err+360;//��ת���Լ�һ�����ڵĽǶ�ֵ��360��
//		res2=ptr->angle_err;
//	}
//	
//	if(ABS(res1)<ABS(res2)) //��������ת���϶���ת�ĽǶ�С���Ǹ������
//	{
//		ptr->angle_err_err = res1;
//	}
//	else
//	{
//		ptr->angle_err_err = res2;
//	}
//	
//	ptr->POS_ABS += ptr->angle_err_err;	
//	ptr->pos_old  = ptr->pos;
//}




///**��������**/
//uint8_t  q_mode=100,e_mode=10,f_mode=1;
//void Shoot_SpeedLimit(void)
//{
//	switch((int)REF_FOOT_Shoot_SpeedLimit())
//  {
//    case 15:
//			PID_calc(&pid_shoot[1],	motor_shoot[1].speed_rpm , 4200);
//			PID_calc(&pid_shoot[2], motor_shoot[2].speed_rpm ,-4200);
//			f_mode=1;
//				break;
//    case 18:
//			PID_calc(&pid_shoot[1],	motor_shoot[1].speed_rpm , 4900);
//			PID_calc(&pid_shoot[2], motor_shoot[2].speed_rpm ,-4900);
//        break;
//    case 30:
//			PID_calc(&pid_shoot[1],	motor_shoot[1].speed_rpm , 7100);
//			PID_calc(&pid_shoot[2], motor_shoot[2].speed_rpm ,-7100);
//			f_mode=2;
//        break;

//		default:
//			PID_calc(&pid_shoot[1],	motor_shoot[1].speed_rpm , 4200);
//			PID_calc(&pid_shoot[2], motor_shoot[2].speed_rpm ,-4200);
//        break;
//	}
//}
//	



//extern float driver_out;
//uint16_t v;
//void shoot_one(void)
//{
//	if(rc_ctrl.mouse.press_l==1)
//	{
//		v++;
//	if(v==1)
//	{
//		moto_shoot[0].ref_pos-=1620;
//	}

//	}
//		else 
//	{
//		v=0;
//	}
//}

//int16_t driver_speed;
////���������ת
//void shoot_pc_G(void)
//{
//	if(rc_ctrl.key.v & KEY_PRESSED_OFFSET_G)
//		{
//			driver_speed=5000;	
//		}
//}

///**�������**/
//void shoot_long(void)
//{		
//	if(rc_ctrl.mouse.press_l==1)
//	{
//		 driver_speed=-4000;	
//	}
//	else
//	{
//		driver_speed=0;
//	}
//}
///**ǹ����������**/
//void Shoot_CoolingLimit(void)
//{
//	if(REF_FOOT_Shooter_Heat()<(REF_FOOT_CoolingLimit()-15))
//	{
//		shoot_long();
//	}
//	else
//	{
//		driver_speed=0;
//	}
//}

//uint16_t shoot_1=0;//����mode
//uint16_t magazine=0;//���ص���
//uint8_t  visual_mode;//��λ 1����   2 �ڱ�   ʮλ 1 ��ͨ   2 Ԥ��    ��λ 1  15m/s    2  30m/s 

///**���տ���**/
//void shoot_pc_R(void)
//{
//	static uint8_t helm=1;
//	if (helm==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_R)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_R)
//			{	
//				helm=0;
//				if(magazine==0)
//				{magazine=1;}
//				else	
//				{magazine=0;}
//			}
//		}
//		if(helm==0&&rc_ctrl.key.v==0)
//		{
//			helm=1;
//		}
//			if(magazine==0)
//		{
//			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,1780);
//		}
//		else
//		{
//			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,850);
//		}	
//}


///**���mode�л�**/
//uint8_t dial_flag=1,dial_flag_flag;
//uint8_t dial_state=DIAL_ONE;
//void shoot_pc_F(void)
//{
//	static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_F)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_F)
//			{	
//				press_flag=0;
//				if(shoot_1==0)
//				{shoot_1=1;}
//				else	
//				{shoot_1=0;}
//				moto_shoot[0].POS_ABS=0;
//				moto_shoot[0].ref_pos=0;
//			}
//			
//		}
//	if(press_flag==0&&rc_ctrl.key.v==0)
//	{
//		press_flag=1;
//	}
//	
//	if(shoot_1==1)
//	{
//		shoot_one();
//	}
//	else
//	{
//		Shoot_CoolingLimit();	/**��������**/
////	shoot_long();
//	}
//}



////��λ   1����  2�ڱ�
//void shoot_pc_Q(void)
//{
//static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_Q)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_Q)
//			{	
//				press_flag=0;
//				if(q_mode==100){
//				q_mode=200;
//				}
//				else {
//				q_mode=100;
//				}
//				
//			}
//		}
//	if(press_flag==0&&rc_ctrl.key.v==0)
//	{
//		press_flag=1;
//	}
//}

////1��ͨ  2Ԥ��
//void shoot_pc_E(void)
//{
//static uint8_t press_flag=1;
//	if (press_flag==1&&rc_ctrl.key.v & KEY_PRESSED_OFFSET_E)
//		{	
//			HAL_Delay(20);		
//			if (rc_ctrl.key.v & KEY_PRESSED_OFFSET_E)
//			{	
//				press_flag=0;
//				if(e_mode==10){
//				e_mode=20;
//				}
//				
//				else	{
//				e_mode=10;
//				}
//			}
//		}
//	if(press_flag==0&&rc_ctrl.key.v==0)
//	{
//		press_flag=1;
//	}
//}

//void shoot_rc_pc(void)
//{	
////	Shoot_CoolingLimit();	/**��������**/
//	Shoot_SpeedLimit();		/**��������**/
//	shoot_pc_F();					/**���ģʽ**/
//	shoot_pc_R();					/**���տ���**/
//	shoot_pc_G();					/**�����ת**/
//	shoot_pc_Q();					/**����  �ڱ�**/
//	shoot_pc_E();					/**����  Ԥ��**/
//}



//void shoot_init(void)
//{
////���̴�������
//	PID_calc(&pid_shoot[3], moto_shoot[0].POS_ABS,moto_shoot[0].ref_pos);//ʵ��  Ŀ��
//	PID_calc(&pid_shoot[4], motor_shoot[3].speed_rpm , pid_shoot[3].out);
////��������
//	PID_calc(&pid_shoot[5], motor_shoot[3].speed_rpm ,driver_speed);
//}

//void shoot_ctrl(void)
//{
//	visual_mode=q_mode+e_mode+f_mode;
//	shoot_init();
//	Motor_Angle_Cal(&moto_shoot[0]);

//	if(rc_ctrl.rc.s1==1)//dbus_shoot
//	{
//		
//		if(rc_ctrl.rc.s2==1)
//		{
//			PID_calc(&pid_shoot[1],	motor_shoot[1].speed_rpm , 4200);
//			PID_calc(&pid_shoot[2], motor_shoot[2].speed_rpm ,-4200);	
//			if(motor_shoot[1].speed_rpm>4000)
//			{
//				driver_speed=-4000;		
//			}
//			else {
//				driver_speed=0;
//			}
//		CAN_cmd_shoot(pid_shoot[1].out,pid_shoot[2].out);		
//		}
//		if(rc_ctrl.rc.s2!=1)
//		{
//			driver_speed=0;
//			CAN_cmd_shoot(0,0);
//		}	
//}

//	if(rc_ctrl.rc.s1==3)
//	{
//		CAN_cmd_shoot(0,0);
//	}

//	if(rc_ctrl.rc.s1==2)//pc
//	{	
//		shoot_rc_pc();
//		CAN_cmd_shoot(pid_shoot[1].out,pid_shoot[2].out);		
//	}
//}
