#ifndef __DRV_USART_H
#define __DRV_USART_H
#include "stm32f4xx_hal.h"


/* ----------------------- Data Struct ------------------------------------- */



/* ----------------------- Internal Data ----------------------------------- */



/* ----------------------- Internal Functions ----------------------------------- */
void USART_Init(void);
void Remote_Control_Init(void);
void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
void UART_IRQHandler_IT(UART_HandleTypeDef *huart);


#endif

