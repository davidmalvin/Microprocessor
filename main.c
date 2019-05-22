/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "gpio.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
	LL_RTC_TimeTypeDef RTC_TimeStruct = {0};
	LL_RTC_DateTypeDef RTC_DateStruct = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void lcdinit4();
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
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

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

  //activation de l'horloge pour GPIOA et GPIOB
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;
  /*Ici la fonction LL_mDelay sera utilisée pour faire des temps  d'attente en us
   Pour cette raison on initialise le nombre de ticks nécessaire pour
  faire 1ms à 16000 au lieu de 16000000
   */
  LL_Init1msTick(16000);

  //Initialisation du LCD
  lcdinit4(); //call lcdinit4

  //les deux lignes qui seront affichées sur le LCD

  uint32_t hour;
  uint32_t minute;
  uint32_t seconde;
  uint32_t weekday;
  uint32_t day;
  uint32_t month;
  uint32_t year;

  char hour_tab[20] = "";
  char minute_tab[20] = "";
  char seconde_tab[20] = "";
  char clock[20] = "";
  char weekday_tab[20] = "";
  char day_tab[20]="";
  char month_tab[20] = "";
  char year_tab[20]="";
  char date[20]="";

  //Affichage sur le LCD
  RTC_TimeStruct.Hours = 13;
  RTC_TimeStruct.Minutes = 16;
  RTC_TimeStruct.Seconds = 0;
  LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStruct);
  RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_FRIDAY;
  RTC_DateStruct.Day= 17;
  RTC_DateStruct.Month = LL_RTC_MONTH_MAY;
  RTC_DateStruct.Year = 19;
  LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_DateStruct);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  hour = LL_RTC_TIME_GetHour(RTC);
	  minute = LL_RTC_TIME_GetMinute(RTC);
	  seconde = LL_RTC_TIME_GetSecond(RTC);
	 weekday= LL_RTC_DATE_GetWeekDay(RTC);
	  day = LL_RTC_DATE_GetDay(RTC);
	  month = LL_RTC_DATE_GetMonth(RTC);
	  year =LL_RTC_DATE_GetYear(RTC);

	  itoa(hour, hour_tab, 10);
	  itoa(minute, minute_tab, 10);
	  char u_s[20];
	  char d_s[20];
	  itoa(seconde&(0b1111),u_s,10);
	  itoa(seconde>>4,d_s,10);

	 strcpy(seconde_tab, d_s);
	 strcat(seconde_tab, u_s);


      itoa(weekday,weekday_tab,10);
      itoa(day,day_tab,10);
      itoa(month, month_tab,10);
      itoa(year,year_tab,10);

	  strcpy(clock, hour_tab);
	  strcat(clock, " : ");
	  strcat(clock, minute_tab);
	  strcat(clock, " : ");
	  strcat(clock, seconde_tab);
	  strcpy(date,weekday_tab);
	  strcat(date," ");
	  strcpy(date,day_tab);
      strcat(date," ");
      strcat(date, month_tab);
      strcat(date," ");
      strcat(date,year_tab);

	  Affichage_LCD(clock, date);


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {
    
  }
  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(16000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(16000000);
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
