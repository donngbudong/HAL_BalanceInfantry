#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H


/** 《遥控器信息图》
 * ┌──────────────────────────────────────────┐
 * │ ┌───┐1  660                                       ┌──┐1                   │
 * │ │S 1   │3   Λ                              Λ       │S 2 │3 			        		 │
 * │ └───┘2       │                              │       └──┘2 					  			 │
 * │               │                              │                                   │
 * │ CH2<─────┼─────>660  CH0<─────┼─────>660                     │
 * │               │                              │                                   │
 * │               │                              │          	                       │
 * │                V                               V                                   │
 * │              CH3                              CH1                                  │
 * └──────────────────────────────────────────┘
 */
#include "stm32f4xx_hal.h"
#include "System.h"

#define abs(x) ((x)>0? (x):(-(x)))       //绝对值宏定义



#define SBUS_RX_BUF_NUM 36u
#define RC_FRAME_LENGTH 18u



/* ----------------------- Data Struct ------------------------------------- */
typedef struct
{
	 struct
	{
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
		uint8_t s1;
		uint8_t s2;
		int16_t sw;
	} rc;
	 struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	} mouse;
	
	union {
    uint16_t key;
    struct
    {
      uint16_t W : 1;
      uint16_t S : 1;
      uint16_t A : 1;
      uint16_t D : 1;
      uint16_t SHIFT : 1;
      uint16_t CTRL : 1;
      uint16_t Q : 1;
      uint16_t E : 1;
      uint16_t R : 1;
      uint16_t F : 1;
      uint16_t G : 1;
      uint16_t Z : 1;
      uint16_t X : 1;
      uint16_t C : 1;
      uint16_t V : 1;
      uint16_t B : 1;
    } bit;
  } kb;
} RC_Ctrl_t;

/* ----------------------- Internal Data ----------------------------------- */
extern RC_Ctrl_t RC_Ctrl;
extern uint64_t Remote_time ;//遥控器

/* ----------------------- RC Channel Definition---------------------------- */
//#define RC_CH_VALUE_MIN ((uint16_t)-660 )
//#define RC_CH_VALUE_OFFSET ((uint16_t)0)
//#define RC_CH_VALUE_MAX ((uint16_t)660)
#define RC_CH0              RC_Ctrl.rc.ch0
#define RC_CH1              RC_Ctrl.rc.ch1
#define RC_CH2              RC_Ctrl.rc.ch2
#define RC_CH3              RC_Ctrl.rc.ch3
#define RC_S1               RC_Ctrl.rc.s1      
#define RC_S2               RC_Ctrl.rc.s2
#define RC_SW               RC_Ctrl.rc.sw

#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
#define RC_SW_ERR ((uint16_t)4)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W            ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R            ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F            ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G            ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z            ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X            ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C            ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V            ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B            ((uint16_t)1 << 15)


/* ----------------------- Internal Functions ----------------------------------- */


void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_Ctrl_t *RC_Ctrl);


void RC_State_Report(void);
Rc_State_t RC_Check(void);

bool Judge_RC_DataErr(void);
bool Judge_RC_Lost(void);




#define    IF_RC_DATAERR   Judge_RC_DataErr()
#define    IF_RC_LOST      Judge_RC_Lost()
#define    IF_RC_NORMAL    !(IF_RC_DATAERR || IF_RC_LOST)

#endif
