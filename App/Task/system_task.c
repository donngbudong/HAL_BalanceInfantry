#include "system_task.h"
#include "tim.h"


#define ONE_PULSE        (142)                           //1 
#define ZERO_PULSE       (67)                           //0 
#define RESET_PULSE      (80)                           //80 
#define LED_NUMS         (6)                            //led 
#define LED_DATA_LEN     (24)                           //led 
#define WS2812_DATA_LEN  (LED_NUMS*LED_DATA_LEN)        //ws2812


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
		HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
}
 


uint16_t static RGB_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };

void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    uint16_t* p = (RGB_buffur + RESET_PULSE) + (num * LED_DATA_LEN);
    
    for (uint16_t i = 0;i < 8;i++)
    {
        p[i]      = (G << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 8]  = (R << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
    }
}
void ws2812_blue(uint8_t led_nums)
{
	uint16_t num_data;
	num_data = 80 + led_nums * 24;
	for(uint8_t i = 0; i < led_nums; i++)
	{
		ws2812_set_RGB(0x00, 0x00, 0x22, i);
	}
	 HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)RGB_buffur,(num_data));
}
uint8_t i=100;
uint8_t x,y,z;
void State_LED(void)
{
	 // X
	 if (x == 0)
	 {
		ws2812_set_RGB(i, 0x00, 0x00, 0);
		ws2812_set_RGB(i, 0x00, 0x00, 1);
	 }
	 else
	 {
		ws2812_set_RGB(0x00, i, 0x00, 0);
		ws2812_set_RGB(0x00, i, 0x00, 1);
	 }
	 // Y
	 if (y == 0)
	 {
		ws2812_set_RGB(0x00, i, 0x00, 2);
		ws2812_set_RGB(0x00, i, 0x00, 3);
	 }
	 else
	 {
		ws2812_set_RGB(0x00, 0x00, i, 2);
		ws2812_set_RGB(0x00, 0x00, i, 3);
	 }
	 // Z
	 if (z == 0)
	 {
		ws2812_set_RGB(0x00, 0x00, i, 4);
		ws2812_set_RGB(0x00, 0x00, i, 5);
	 }
	 else
	 {
		ws2812_set_RGB(i, 0x00, 0x00, 4);
		ws2812_set_RGB(i, 0x00, 0x00, 5);
	 }

//    ws2812_set_RGB(0x00, 0x22, 0x00, 1);
//    ws2812_set_RGB(0x00, 0x00, 0x22, 2);
//    ws2812_set_RGB(0x22, 0x00, 0x00, 3);
//    ws2812_set_RGB(0x00, 0x22, 0x00, 4);
//    ws2812_set_RGB(0x00, 0x00, 0x22, 5);
//		ws2812_set_RGB(0x22, 0x00, 0x00, 6);
//    ws2812_set_RGB(0x00, 0x22, 0x00, 7);
    HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)RGB_buffur,(232)); //272 = 80 + 24 * LED_NUMS(6)
//    HAL_Delay(50);
}
