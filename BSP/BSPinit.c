/* 
 * BSPinit.c - The C source file of the system Board device init
 * NOTE: This file is based on HAL library of stm32 platform
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-04     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"
#include "uartFinshCharIO.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
extern void SystemClock_Config(void);

void myBSPInit(void)
{
  /**Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
    
  /**USER CODE BEGIN Init */

  /**USER CODE END Init */

  /**Configure the system clock */
  SystemClock_Config();

  /**USER CODE BEGIN SysInit */

  /**USER CODE END SysInit */

  /**Initialize all configured peripherals */
  MX_GPIO_Init();
  
  MX_DMA_Init();
    
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
    
  MX_TIM2_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  
  /**Init FinSH */
  FinshIoInit();
}

INIT_BOARD_EXPORT(myBSPInit);

