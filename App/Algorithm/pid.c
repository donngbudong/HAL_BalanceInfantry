#include "pid.h"
#include "Device.h"


//反限幅（死区）
float anti_constrain(float amt,float high,float low)
{
	if (amt > low && amt < high)
		return 0;
	else
		return amt;
}


/* 用于调试PID参数的函数 */
PID_Parameter_t PID_SpeedDebug = {
  .P = 0,
  .I = 0,
  .D = 0,
};
PID_Parameter_t PID_AngleDebug = {
  .P = 0,
  .I = 0,
  .D = 0,
};
void PID_Debug(PID_Info_t *str)
{
  str->Speed_Loop.PID_Param.P = PID_SpeedDebug.P;
  str->Speed_Loop.PID_Param.I = PID_SpeedDebug.I;
  str->Speed_Loop.PID_Param.D = PID_SpeedDebug.D;
  
  str->Angle_Loop.PID_Param.P = PID_AngleDebug.P;
  str->Angle_Loop.PID_Param.I = PID_AngleDebug.I;
  str->Angle_Loop.PID_Param.D = PID_AngleDebug.D;
}
			
/**
  * @brief  PID(速度/角度)环控制器
  * @param  目标-真实
  * @retval None
  */
float PID_Position(PID_Loop_t *pid, float target, float actual)
{
	pid->Target=target;
	pid->Actual=actual;
	//误差=目标值-实际值
	pid->Err = target - actual;
  	/*死区*/
  pid->Err = anti_constrain(pid->Err,pid->PID_Err_Dead,-pid->PID_Err_Dead);
	//积分限幅
	pid->I_Limit = pid->I_Limit + pid->Err;
	LimitMax(pid->I_Limit,pid->I_Limit_Max);

	pid->PID_P_Out = pid->PID_Param.P * pid->Err;
	pid->PID_I_Out = pid->PID_Param.I * pid->I_Limit;
	pid->PID_D_Out = pid->PID_Param.D * (pid->Err - pid->Last_Err);
	//I输出限幅
	LimitMax(pid->PID_I_Out,pid->PID_I_Out_Max);
	pid->PID_Output = pid->PID_P_Out+pid->PID_I_Out+pid->PID_D_Out;
	
	pid->Last_Err = pid->Err;
	//总输出限幅
	LimitMax(pid->PID_Output, pid->PID_Output_Max);
	
	return pid->PID_Output;
}

PID_Parameter_t Param_3 = {50,	0,	0};
PID_Parameter_t Param_4 = {2	,	0,	0};
PID_Parameter_t Param_5 = {1,	0,	20};
PID_Parameter_t Param_6 = {2	,	0,	0};

PID_Parameter_t Chasssis_OUT = {1,0,0};

PID_Parameter_t PID_Speed_Param[MOTOR_TYPE_CNT][PID_TYPE_CNT]={
	  [GM_6020_YAW] = {
		 [Clear_Away]   = {
      .P = 0,
      .I = 0,
      .D = 0,
    },
    [RC]   = {
      .P = 160,
      .I = 0.5,
      .D = 0,
    },
		[KEY]	 = {
      .P = 5,
      .I = 0,
      .D = 0,
		},
	},
	  [GM_6020_PIT] = {
		 [Clear_Away]   = {
      .P = 0,
      .I = 0,
      .D = 0,
    },
    [RC]   = {
      .P = 180,
      .I = 2,
      .D = 0,
    },
		[KEY]	 = {
      .P = 5,
      .I = 0,
      .D = 0,
		},
	},
};

PID_Parameter_t PID_Angle_Param[MOTOR_TYPE_CNT][PID_TYPE_CNT]={
		[GM_6020_YAW] = {
		 [Clear_Away]   = {
      .P = 0,
      .I = 0,
      .D = 0,
    },
    [RC]   = {
      .P = 15,
      .I = 0,
      .D = 0,
    },
		[KEY]	 = {
      .P = 5,
      .I = 0,
      .D = 0,
		},
	},
	  [GM_6020_PIT] = {
		 [Clear_Away]   = {
      .P = 0,
      .I = 0,
      .D = 0,
    },
    [RC]   = {
      .P = 10,
      .I = 0,
      .D = 0,
    },
		[KEY]	 = {
      .P = 5,
      .I = 0,
      .D = 0,
		},
	},
};


/**
 * @brief 总电机初始化
 * @param 
 */
void Motor_Init(void)
{
  MF_9025_Init();
	GM_6020_Init();
}


void MF_9025_Init(void)
{
	Chassis.Motor_Info.Motor_Type = MF_9025_Right;
	MF_9028_Balance_Kp(&Chassis.PID.PID_b3);
	MF_9028_Balance_Kd(&Chassis.PID.PID_b4);
	MF_9028_PID_56(&Chassis.PID.PID_b5);
	MF_9028_PID_56(&Chassis.PID.PID_b6);

	MF_9028_Chasssis_OUT(&Chassis.PID.Chasssis_OUT);

}



/**
 * @brief PID限幅设置 
 * @param 
 */
void MF_9028_Balance_Kp(PID_Loop_t *str)
{
  /* 这里只针对速度环 */
  str->I_Limit_Max = 10000;
  str->PID_I_Out_Max = 8000; 
  str->PID_P_Out_Max = 16000;
  str->PID_Output_Max = 16000;
  str->PID_Err_Dead = 0;
}

/**
 * @brief PID限幅设置 
 * @param 
 */
void MF_9028_Balance_Kd(PID_Loop_t *str)
{
  /* 这里只针对速度环 */
  str->I_Limit_Max = 10000;
  str->PID_I_Out_Max = 0; 
  str->PID_P_Out_Max = 16000;
  str->PID_Output_Max = 16000;
  str->PID_Err_Dead = 0;
}

/**
 * @brief PID限幅设置 
 * @param 
 */
void MF_9028_PID_56(PID_Loop_t *str)
{
  /* 这里只针对速度环 */
  str->I_Limit_Max = 2000;
  str->PID_I_Out_Max = 0; 
  str->PID_P_Out_Max = 16000;
  str->PID_Output_Max = 12000;
  str->PID_Err_Dead = 0;
}
/**
 * @brief PID限幅设置 
 * @param 
 */
void MF_9028_Chasssis_OUT(PID_Loop_t *str)
{
  /* 这里只针对速度环 */
  str->I_Limit_Max = 10000;
  str->PID_I_Out_Max = 30; 
  str->PID_P_Out_Max = 50000;
  str->PID_Output_Max = 2000;
  str->PID_Err_Dead = 0;
}





/*云台GM6020电机--------------------------------------*/
/**
 * @brief Yaw轴的PID限幅设置
 * @param 
 */
void GIM_YawParamInit(Gimbal_Info_t *str)
{
  /* 速度环 */
  str->PID.Speed_Loop.I_Limit_Max = 50000;
  str->PID.Speed_Loop.PID_I_Out_Max = 25000;
  str->PID.Speed_Loop.PID_P_Out_Max = 50000;
  str->PID.Speed_Loop.PID_Output_Max = 25000;
  str->PID.Speed_Loop.PID_Err_Dead = 0;
  /* 角度环 */
  str->PID.Angle_Loop.I_Limit_Max = 12000;
  str->PID.Angle_Loop.PID_I_Out_Max = 25000;
  str->PID.Angle_Loop.PID_P_Out_Max = 50000;
  str->PID.Angle_Loop.PID_Output_Max = 13000;
  str->PID.Angle_Loop.PID_Err_Dead = 0;  
  /* 遥控模式 */
  str->RC_Move.Angle_Inc = 0;
  str->RC_Move.Angle_k = 0.0005;
  str->RC_Move.Slow_Inc = 0;
	
}

/**
 * @brief Pitch轴的PID限幅设置
 * @param 
 */
void GIM_PitParamInit(Gimbal_Info_t *str)
{
  /* 速度环 */
  str->PID.Speed_Loop.I_Limit_Max = 50000;
  str->PID.Speed_Loop.PID_I_Out_Max = 25000;
  str->PID.Speed_Loop.PID_P_Out_Max = 50000;
  str->PID.Speed_Loop.PID_Output_Max = 25000;
  str->PID.Speed_Loop.PID_Err_Dead = 0;
  /* 角度环 */
  str->PID.Angle_Loop.I_Limit_Max = 12000;
  str->PID.Angle_Loop.PID_I_Out_Max = 25000;
  str->PID.Angle_Loop.PID_P_Out_Max = 50000;
  str->PID.Angle_Loop.PID_Output_Max = 13000;
  str->PID.Angle_Loop.PID_Err_Dead = 0;  
  /* 遥控模式 */
  str->RC_Move.Angle_Inc = 0;
  str->RC_Move.Angle_k = 0.001;
  str->RC_Move.Slow_Inc = 0;
}

/**
 * @brief 云台电机初始化
 * @param 
 */
void GM_6020_Init(void)
{
  Gimbal.YAW.Motor_Info.Motor_Type = GM_6020_YAW;
  Gimbal.PITCH.Motor_Info.Motor_Type = GM_6020_PIT;
  
  GIM_YawParamInit(&Gimbal.YAW);
  GIM_PitParamInit(&Gimbal.PITCH);
}
