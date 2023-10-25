#ifndef _VOFA_TASK_H
#define _VOFA_TASK_H
 #include "stm32f4xx_hal.h"





/*ÁªºÏÌå*/
typedef union 
{
	float		receive;
	uint8_t	send[4];
}Vofa_Date; 



typedef struct
{
	
	Vofa_Date	Chassis_LF;
	Vofa_Date	Chassis_RF;
	Vofa_Date	Chassis_LB;
	Vofa_Date	Chassis_RB;
}Vofa_Motor_t;
//void vofa_send(void);

#endif

