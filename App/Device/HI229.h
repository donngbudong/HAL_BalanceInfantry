#ifndef __HI229_H
#define __HI229_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include "packet.h"
#include "System.h"

typedef enum
{
	PITCH = 0,
	ROLL  = 1,
	YAW  	= 2,
	EULAR_CNT =3
}Eluar;

typedef enum
{
	X  = 0,
	Y  = 1,
	Z  = 2,
	Gyo_CNT =3
}Gyo;

/*IMU接收到的数据 结构体*/
typedef  struct
{
  float   IMU_Eular[EULAR_CNT];
  int16_t IMU_Gyo[Gyo_CNT];
}IMU_GET_DATA_t;


/* Exported functions --------------------------------------------------------*/
int IMU_Init(void);
int get_rawacc_acc(int16_t* a);
int get_raw_gyo(int16_t* g);
int get_raw_mag(int16_t* m);
int get_id(uint8_t *user_id);
int get_eular(float* e);
int get_quat(float* q);
void IMU_get(void);

void IMU_State_Report(void);

Imu_State_t IMU_Check(void);
bool Judge_IMU_Lost(void);

#define    IF_IMU_LOST      Judge_IMU_Lost()



#endif
