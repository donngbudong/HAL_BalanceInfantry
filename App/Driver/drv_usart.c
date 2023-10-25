#include "Device.h"
#include "system.h"

uint8_t Judge_Buffer[200] = {0};
uint8_t Dbus_Buffer[2][SBUS_RX_BUF_NUM];
uint8_t Visual_Buffer[13];	//�������ͳ���
uint8_t IMU_Buffer  [128];




extern UART_HandleTypeDef huart1;/*����ϵͳ*/
extern UART_HandleTypeDef huart2;/*ң����*/
extern UART_HandleTypeDef huart4;/*�Ӿ�*/
extern UART_HandleTypeDef huart5;/*������*/
extern UART_HandleTypeDef huart6;/*Vofa*/

extern DMA_HandleTypeDef hdma_usart2_rx;


//static void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

/**
  * @brief          UART��ʼ��
  * @param[in]      none
  * @retval         none
  */
void USART_Init(void)
{
	Remote_Control_Init();//Remote_Control
	IMU_Init();//
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//Referee_System
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);//ң����
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);//Vision	
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);//Imu
	__HAL_UART_ENABLE_IT(&huart6,UART_IT_RXNE);//vofa	
}

/**
  * @brief          ң������ʼ��
  * @param[in]      none
  * @retval         none
  */
void Remote_Control_Init(void)
{
	RC_Init(Dbus_Buffer[0],Dbus_Buffer[1],SBUS_RX_BUF_NUM);
}



void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	//ʹ��DMA���ڽ���
	SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
	//ʹ�ܿ����ж�
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
   //ʧЧDMA
	__HAL_DMA_DISABLE(&hdma_usart2_rx);
	while(hdma_usart2_rx.Instance->CR & DMA_SxCR_EN)
	{
			__HAL_DMA_DISABLE(&hdma_usart2_rx);
	}
	hdma_usart2_rx.Instance->PAR = (uint32_t) & (USART2->DR);
    //�ڴ滺����1
	hdma_usart2_rx.Instance->M0AR = (uint32_t)(rx1_buf);
    //�ڴ滺����2
	hdma_usart2_rx.Instance->M1AR = (uint32_t)(rx2_buf);
    //���ݳ���
	hdma_usart2_rx.Instance->NDTR = dma_buf_num;
   //ʹ��˫������
	SET_BIT(hdma_usart2_rx.Instance->CR, DMA_SxCR_DBM);
    //ʹ��DMA
	__HAL_DMA_ENABLE(&hdma_usart2_rx);
		
}

/**
  * @brief          �ж�IT
  * @param[in]      huart
  * @retval         none
  */
 uint16_t rx_len = 0;
void UART_IRQHandler_IT(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)//Referee_System
	{
		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!=RESET) 
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
			HAL_UART_DMAStop(&huart1);
			HAL_UART_Receive_DMA(&huart1,Judge_Buffer,128);
			Judege_read_data(Judge_Buffer);
			memset(Judge_Buffer, 0, 128);//������ջ���
		}
	}
	else if(huart==&huart2)//Remote_Control
	{
		if(huart2.Instance->SR & UART_FLAG_RXNE)//���յ�����
    {
			__HAL_UART_CLEAR_PEFLAG(&huart2);
    }
		 else if(huart2.Instance->SR & UART_FLAG_IDLE)
		{
			__HAL_UART_CLEAR_PEFLAG(&huart2);

			static uint16_t this_time_rx_len = 0;
			
			if ((huart2.hdmarx->Instance->CR & DMA_SxCR_CT) == RESET)
			{
				/* Current memory buffer used is Memory 0 */
				//ʧЧDMA
				__HAL_DMA_DISABLE(huart2.hdmarx);
        		//��ȡ�������ݳ���,���� = �趨���� - ʣ�೤��
				this_time_rx_len = SBUS_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
				//�����趨���ݳ���
				__HAL_DMA_SET_COUNTER(huart2.hdmarx, SBUS_RX_BUF_NUM);
				//�趨������1
				huart2.hdmarx->Instance->CR |= DMA_SxCR_CT;
				//ʹ��DMA
				__HAL_DMA_ENABLE(huart2.hdmarx);
				if(this_time_rx_len == RC_FRAME_LENGTH)
				{
						sbus_to_rc(Dbus_Buffer[0], &RC_Ctrl);
				}
			}
			else
			{
				/* Current memory buffer used is Memory 1 */
				//ʧЧDMA
					__HAL_DMA_DISABLE(huart2.hdmarx);
        		//��ȡ�������ݳ���,���� = �趨���� - ʣ�೤��
					this_time_rx_len = SBUS_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
				//�����趨���ݳ���
					__HAL_DMA_SET_COUNTER(huart2.hdmarx, SBUS_RX_BUF_NUM);
				//�趨������0
					huart2.hdmarx->Instance->CR &= ~(DMA_SxCR_CT);
				//ʹ��DMA
					__HAL_DMA_ENABLE(huart2.hdmarx);
				if(this_time_rx_len == RC_FRAME_LENGTH)
				{
						//����ң��������
					sbus_to_rc(Dbus_Buffer[1], &RC_Ctrl);
				}
			}
		Remote_time=micros() + 30000;
	 }
	}
	else if(huart==&huart4)//Vision
	{
		if(__HAL_UART_GET_FLAG(&huart4,UART_FLAG_IDLE)!=RESET) 
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart4);
			HAL_UART_DMAStop(&huart4);
//			visual_date(Visual_Buffer);								//��ȡ�Ӿ�����
			HAL_UART_Receive_DMA(&huart4,Visual_Buffer,16);
//			memset(Visual_Buffer, 0, 16);
		}
	}
	else if(huart==&huart5)//Imu
	{
		 if(__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE)!=RESET)   //�ж��Ƿ��ǿ����ж�
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart5);                    		 //��������жϱ�־�������һֱ���Ͻ����жϣ�
			HAL_UART_DMAStop(&huart5);
			//imu���ݽ���
			for(size_t i=0;i<128;i++){
				Packet_Decode(IMU_Buffer[i]);
			}
			HAL_UART_Receive_DMA(huart,IMU_Buffer,128);	
			memset(IMU_Buffer, 0, 128);
			Imu_time=micros() + 30000;
		}
	}

}

