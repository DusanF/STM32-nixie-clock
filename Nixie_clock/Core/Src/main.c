/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "nixie.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  HAL_PWREx_EnableBatteryCharging(PWR_BATTERY_CHARGING_RESISTOR_1_5);	//enable Vbat charging (1k5)
  HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, RTC_SMOOTHCALIB_PLUSPULSES_RESET, 70);	//kalibracia rtc -66.8ppm
  	  	  	  	  	  	  	  	  //1+(-70/(2^20 +70))

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  RTC_TimeTypeDef curr_time = {0};
  RTC_DateTypeDef curr_date = {0};
  uint8_t display[4];	//digits to display, >=10 = blank
  uint8_t display_old[4]={0};
  uint8_t old_mins=0, old_hours=0x25;	//force refresh

  uint8_t btn_a_cnt=0;
  uint8_t btn_b_cnt=0;

  for(uint8_t i=0; i<10; i++){	//briefly light up every digit
	  display[0] = i;
	  display[1] = i;
	  display[2] = i;
	  display[3] = i;
	  nixie_display(display);
	  HAL_Delay(150);
  }
  display[0] = 11;
  display[1] = 11;
  display[2] = 11;
  display[3] = 11;
  nixie_display(display);

  HAL_Delay(500);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(HAL_GPIO_ReadPin(BTN_A_GPIO_Port, BTN_A_Pin))	//check buttons (active low)
		  btn_a_cnt=0;
	  else if(btn_a_cnt<6)	//count up to 6 (loop period = 100ms, long press if cnt>5 (500ms))
		  btn_a_cnt++;

	  if(HAL_GPIO_ReadPin(BTN_B_GPIO_Port, BTN_B_Pin))
		  btn_b_cnt=0;
	  else if(btn_b_cnt<6)
		  btn_b_cnt++;

	  HAL_RTC_GetTime(&hrtc, &curr_time, RTC_FORMAT_BCD);
	  HAL_RTC_GetDate(&hrtc, &curr_date, RTC_FORMAT_BCD);	//need to read date even when not used because of buffered registers



	  if(btn_b_cnt == 1 || btn_b_cnt == 6){		//ignore 2-5 to allow short press of button
		  curr_time.Minutes = RTC_ByteToBcd2((RTC_Bcd2ToByte(curr_time.Minutes) + 1) % 60);
		  curr_time.Seconds = 0;
		  HAL_RTC_SetTime(&hrtc, &curr_time, RTC_FORMAT_BCD);
	  }
	  if(btn_a_cnt == 1 || btn_a_cnt == 6){
		  curr_time.Hours = RTC_ByteToBcd2((RTC_Bcd2ToByte(curr_time.Hours) + 1) % 24);
		  curr_time.Seconds = 0;
		  HAL_RTC_SetTime(&hrtc, &curr_time, RTC_FORMAT_BCD);
	  }
#ifdef FORMAT_12H
	  curr_time.Hours = RTC_ByteToBcd2(((RTC_Bcd2ToByte(curr_time.Hours)) % 12) + 1);
#endif

	  if(old_mins != curr_time.Minutes || old_hours != curr_time.Hours){
		  display[0] = (curr_time.Hours & 0xF0) ? (curr_time.Hours >> 4) : (0xF);	//display only single digit of hours unless needed
		  display[1] = curr_time.Hours & 0xF;
		  display[2] = curr_time.Minutes >> 4;
		  display[3] = curr_time.Minutes & 0xF;

		  if(btn_a_cnt == 0 && btn_b_cnt == 0)	//if not setting time, do CPP - cathode poisoning prevention
			  nixie_cpp(display, display_old);

		  nixie_display(display);

		  memcpy(display_old, display, 4);
		  old_mins = curr_time.Minutes;
		  old_hours = curr_time.Hours;
	  }
	  HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
