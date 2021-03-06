/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_hal.h"

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
#define NIX_4A_Pin GPIO_PIN_0
#define NIX_4A_GPIO_Port GPIOA
#define NIX_4B_Pin GPIO_PIN_1
#define NIX_4B_GPIO_Port GPIOA
#define NIX_4C_Pin GPIO_PIN_2
#define NIX_4C_GPIO_Port GPIOA
#define NIX_4D_Pin GPIO_PIN_3
#define NIX_4D_GPIO_Port GPIOA
#define NIX_3A_Pin GPIO_PIN_4
#define NIX_3A_GPIO_Port GPIOA
#define NIX_3B_Pin GPIO_PIN_5
#define NIX_3B_GPIO_Port GPIOA
#define NIX_3C_Pin GPIO_PIN_6
#define NIX_3C_GPIO_Port GPIOA
#define NIX_3D_Pin GPIO_PIN_7
#define NIX_3D_GPIO_Port GPIOA
#define NIX_2A_Pin GPIO_PIN_0
#define NIX_2A_GPIO_Port GPIOB
#define NIX_2B_Pin GPIO_PIN_1
#define NIX_2B_GPIO_Port GPIOB
#define NIX_2C_Pin GPIO_PIN_2
#define NIX_2C_GPIO_Port GPIOB
#define NIX_2D_Pin GPIO_PIN_12
#define NIX_2D_GPIO_Port GPIOB
#define NIX_1A_Pin GPIO_PIN_13
#define NIX_1A_GPIO_Port GPIOB
#define NIX_1B_Pin GPIO_PIN_14
#define NIX_1B_GPIO_Port GPIOB
#define NIX_1C_Pin GPIO_PIN_15
#define NIX_1C_GPIO_Port GPIOB
#define NIX_1D_Pin GPIO_PIN_8
#define NIX_1D_GPIO_Port GPIOA
#define BTN_B_Pin GPIO_PIN_0
#define BTN_B_GPIO_Port GPIOD
#define BTN_A_Pin GPIO_PIN_1
#define BTN_A_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
