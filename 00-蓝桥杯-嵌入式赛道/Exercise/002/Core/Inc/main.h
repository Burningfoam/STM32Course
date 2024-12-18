/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define LED6_LCD13_Pin GPIO_PIN_13
#define LED6_LCD13_GPIO_Port GPIOC
#define LED7_LCD14_Pin GPIO_PIN_14
#define LED7_LCD14_GPIO_Port GPIOC
#define LED8_LCD15_Pin GPIO_PIN_15
#define LED8_LCD15_GPIO_Port GPIOC
#define B4_Pin GPIO_PIN_0
#define B4_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOB
#define B2_Pin GPIO_PIN_1
#define B2_GPIO_Port GPIOB
#define B3_Pin GPIO_PIN_2
#define B3_GPIO_Port GPIOB
#define LED1_LCD8_Pin GPIO_PIN_8
#define LED1_LCD8_GPIO_Port GPIOC
#define LED2_LCD9_Pin GPIO_PIN_9
#define LED2_LCD9_GPIO_Port GPIOC
#define LED3_LCD10_Pin GPIO_PIN_10
#define LED3_LCD10_GPIO_Port GPIOC
#define LED4_LCD11_Pin GPIO_PIN_11
#define LED4_LCD11_GPIO_Port GPIOC
#define LED5_LCD12_Pin GPIO_PIN_12
#define LED5_LCD12_GPIO_Port GPIOC
#define HC573_Pin GPIO_PIN_2
#define HC573_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
