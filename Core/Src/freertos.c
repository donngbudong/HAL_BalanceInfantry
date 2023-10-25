/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Device.h"
#include "tim.h"
#include "usart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId system_state_taHandle;
osThreadId chassis_taskHandle;
osThreadId gimbal_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void SYSTEM_STATE_TASK(void const * argument);
void CHASSIS_TASK(void const * argument);
void GIMBAL_TASK(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of system_state_ta */
  osThreadDef(system_state_ta, SYSTEM_STATE_TASK, osPriorityRealtime, 0, 256);
  system_state_taHandle = osThreadCreate(osThread(system_state_ta), NULL);

  /* definition and creation of chassis_task */
  osThreadDef(chassis_task, CHASSIS_TASK, osPriorityHigh, 0, 256);
  chassis_taskHandle = osThreadCreate(osThread(chassis_task), NULL);

  /* definition and creation of gimbal_task */
  osThreadDef(gimbal_task, GIMBAL_TASK, osPriorityHigh, 0, 256);
  gimbal_taskHandle = osThreadCreate(osThread(gimbal_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {	
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_SYSTEM_STATE_TASK */
/**
* @brief Function implementing the system_state_ta thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SYSTEM_STATE_TASK */
void SYSTEM_STATE_TASK(void const * argument)
{
  /* USER CODE BEGIN SYSTEM_STATE_TASK */
  /* Infinite loop */
//  uint32_t currentTime;
  for(;;)
  {    
//    currentTime = xTaskGetTickCount();//当前系统时间
    System_State();
    RC_State_Report();
		IMU_State_Report();
		vTaskDelay(1);

//    vTaskDelayUntil(&currentTime, 1);//绝对延时
  }
  /* USER CODE END SYSTEM_STATE_TASK */
}
/* USER CODE BEGIN Header_CHASSIS_TASK */
/**
* @brief Function implementing the chassis_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CHASSIS_TASK */
void CHASSIS_TASK(void const * argument)
{
  /* USER CODE BEGIN CHASSIS_TASK */
  /* Infinite loop */
	uint32_t currentTime;
  for(;;)
  {
    currentTime = xTaskGetTickCount();//当前系统时间
    Chassis_Task();
    vTaskDelayUntil(&currentTime, 3);//绝对延时    
  }
  /* USER CODE END CHASSIS_TASK */
}

/* USER CODE BEGIN Header_GIMBAL_TASK */
/**
* @brief Function implementing the gimbal_task thread.
* @param argument: Not used
* @retval None
*/
uint8_t test[2]={0x01,0x02};
/* USER CODE END Header_GIMBAL_TASK */
void GIMBAL_TASK(void const * argument)
{
  /* USER CODE BEGIN GIMBAL_TASK */
  /* Infinite loop */
	uint32_t currentTime;
  for(;;)
  {
		currentTime = xTaskGetTickCount();//当前系统时间
		IMU_get();
		Gimbal_Task();
//		HAL_UART_Transmit(&huart4,&test[0],1,100);
    vTaskDelayUntil(&currentTime, 3);//绝对延时   
	}
  /* USER CODE END GIMBAL_TASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
