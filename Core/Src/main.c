/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

#include"mytime.h"

#include <time.h>

#define TEST_TIMESTAMP 	 	1704067195 // 31.12.2023 23:59:55 at GMT = 0
#define TEST_TIMESTAMP2		1705924410
//#define TEST_TIMESTAMP		0

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

bool convertDateTypes(date_t *myDate, const struct tm *libDate) //convert from struct tm to date_t
{
    myDate->Year = (uint8_t)libDate->tm_year - 70;
    myDate->Month = (uint8_t)libDate->tm_mon;
    myDate->Day = (uint8_t)libDate->tm_mday;

    myDate->Hour = (uint8_t)libDate->tm_hour;
    myDate->Minutes = (uint8_t)libDate->tm_min;
    myDate->Seconds = (uint8_t)libDate->tm_sec;

    return true;

}
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  char uartStr[64];
  char *uartStr_ptr = &uartStr;
  char tempStr[64];
  char *tempStr_ptr = &tempStr;

  date_t myDate;
  struct tm *libDate;

  timestamp_t lastTimestamp;
  timestamp_t myTimestampA = TEST_TIMESTAMP;
  timestamp_t myTimestampB = TEST_TIMESTAMP2;
  timestamp_t myTimestampC;

  //time_t libTimestamp = TEST_TIMESTAMP;

  weekday_t day;
  uint16_t elapsedDays;

  bool ReturnValue;
  bool isOk = true;

  //test sending NULLs

  ReturnValue = timeToDate(myTimestampA, NULL);
  if(ReturnValue != 0) isOk = false;

  ReturnValue = dateToTime(NULL, NULL);
  if(ReturnValue != 0) isOk = false;

  ReturnValue = dateToTime(&myDate, NULL);
  if(ReturnValue != 0) isOk = false;

  ReturnValue = dateToTime(NULL, myTimestampA);
  if(ReturnValue != 0) isOk = false;

  ReturnValue = timeToWeekday(myTimestampA, NULL);
  if(ReturnValue != 0) isOk = false;

  ReturnValue = dayBetweenTimes(myTimestampA, myTimestampB, NULL);
  if(ReturnValue != 0) isOk = false;

  sprintf(uartStr_ptr, "isOk value for NULL tests: %d\n", (uint8_t)isOk);
  HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  //test sending TIMESTAMP

	  if(myTimestampA != lastTimestamp)
	  {
		  //time to date
		   ReturnValue = timeToDate(myTimestampA, tempStr_ptr);
		   sprintf(uartStr_ptr, "timestamp A time is: %s\n", tempStr_ptr);
		   HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);
		   if(ReturnValue == 0) isOk = false;

		   ReturnValue = timeToDate(myTimestampB, tempStr_ptr);
		   sprintf(uartStr_ptr, "timestamp B time is: %s\n", tempStr_ptr);
		   HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);
		   if(ReturnValue == 0) isOk = false;


		   //date to time
		   time_t temp = (time_t)myTimestampA;
		   libDate = localtime(&temp); //convert from timestamp to struct tm
		   convertDateTypes(&myDate, libDate); //convert from struct tm to  date_t

		   sprintf(uartStr_ptr, "timestamp A value is: %d\n", myTimestampA);
		   HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);

		   ReturnValue = dateToTime(&myDate, &myTimestampC);
		   sprintf(uartStr_ptr, "dateToTime value is: %d\n", myTimestampC);
		   HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);

		   if((ReturnValue == 0) || ((myTimestampC - myTimestampA) != 0)) isOk = false;

		   //time to weekday
		   ReturnValue = timeToWeekday(myTimestampA, &day);
		   sprintf(uartStr_ptr, "for timestamp A date current day is %d\n", (uint8_t)day);
		   HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);
		   if(ReturnValue == 0) isOk = false;

		  //days between times
		  ReturnValue = dayBetweenTimes(myTimestampA, myTimestampB, &elapsedDays);
		  sprintf(uartStr_ptr, "elapsed days between timestamp A and B are: %d\n", elapsedDays);
		  HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);
		  if(ReturnValue == 0) isOk = false;

		  sprintf(uartStr_ptr, "isOk value for upper tests is: %d\n\n", (uint8_t)isOk);
		  HAL_UART_Transmit(&huart3, uartStr_ptr, strlen(uartStr_ptr), 500);

		  lastTimestamp = myTimestampA;
	  }


	  HAL_Delay(800);
	  myTimestampA += 86000; //not whole day


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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
