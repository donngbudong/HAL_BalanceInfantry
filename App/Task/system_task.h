#ifndef __SYSTEM_TASK_H
#define	__SYSTEM_TASK_H
#include "stdint.h"
#define ONE_PULSE        (142)                           //1 
#define ZERO_PULSE       (67)                           //0 
#define RESET_PULSE      (80)                           //80 
#define LED_NUMS         (6)                            //led 
#define LED_DATA_LEN     (24)                           //led 
#define WS2812_DATA_LEN  (LED_NUMS*LED_DATA_LEN)        //ws2812


void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);
void ws2812_blue(uint8_t led_nums);
void State_LED(void);



#endif
