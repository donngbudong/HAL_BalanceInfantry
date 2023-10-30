#ifndef __SYSTEM_TASK_H
#define	__SYSTEM_TASK_H
#include "stdint.h"

void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);
void ws2812_blue(uint8_t led_nums);
void State_LED(void);



#endif
