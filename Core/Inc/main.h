/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define POS2_EXTI3_Pin GPIO_PIN_3
#define POS2_EXTI3_GPIO_Port GPIOE
#define POS2_EXTI3_EXTI_IRQn EXTI3_IRQn
#define POS3_EXTI4_Pin GPIO_PIN_4
#define POS3_EXTI4_GPIO_Port GPIOE
#define POS3_EXTI4_EXTI_IRQn EXTI4_IRQn
#define POS4_EXTI13_Pin GPIO_PIN_13
#define POS4_EXTI13_GPIO_Port GPIOC
#define POS4_EXTI13_EXTI_IRQn EXTI15_10_IRQn
#define SCANO_EXTI_Pin GPIO_PIN_2
#define SCANO_EXTI_GPIO_Port GPIOA
#define SCANO_EXTI_EXTI_IRQn EXTI2_IRQn
#define detector_GPI_Pin GPIO_PIN_3
#define detector_GPI_GPIO_Port GPIOA
#define SCANO_EXTI5_Pin GPIO_PIN_5
#define SCANO_EXTI5_GPIO_Port GPIOA
#define SCANO_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define LED_1_Pin GPIO_PIN_2
#define LED_1_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_7
#define LED_2_GPIO_Port GPIOE
#define FINSH_UART3_TX_Pin GPIO_PIN_8
#define FINSH_UART3_TX_GPIO_Port GPIOD
#define FINSH_UART3_RX_Pin GPIO_PIN_9
#define FINSH_UART3_RX_GPIO_Port GPIOD
#define HOST_USART1_TX_Pin GPIO_PIN_9
#define HOST_USART1_TX_GPIO_Port GPIOA
#define HOST_USART1_RX_Pin GPIO_PIN_10
#define HOST_USART1_RX_GPIO_Port GPIOA
#define PWM_3_Pin GPIO_PIN_3
#define PWM_3_GPIO_Port GPIOB
#define PWM_2_Pin GPIO_PIN_8
#define PWM_2_GPIO_Port GPIOB
#define PWM_1_Pin GPIO_PIN_9
#define PWM_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
