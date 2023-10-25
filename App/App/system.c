#include "System.h"


__STATIC_INLINE uint32_t GXT_SYSTICK_IsActiveCounterFlag(void)
{
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}
static uint32_t getCurrentMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  GXT_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (GXT_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}
//获取系统时间，单位us
uint32_t micros(void)
{
  return getCurrentMicros();
}


Sys_Info_t System = {
    .Rc_State  = RC_LOST,
    .Imu_State = IMU_LOST,
    .System_State = SYSTEM_LOST,
};

uint64_t Remote_time = 0;//遥控器
uint64_t Imu_time = 0;//裁判系统

//设备连接初始化
void Time_Init(void)
{
  Remote_time = micros();
  Imu_time = micros();
}



void System_State(void)
{
    if(System.Rc_State ==RC_ERR || System.Imu_State ==IMU_ERR)
  {
    System.System_State = SYSTEM_ERR;
  } 
    if(System.Rc_State ==RC_NORMAL && System.Imu_State ==IMU_NORMAL)
  {
    System.System_State = SYSTEM_NORMAL;
  }
  if(System.Rc_State ==RC_LOST || System.Imu_State ==IMU_LOST)
  {
    System.System_State = SYSTEM_LOST;
  }
}

