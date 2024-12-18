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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
uint16_t adc_val[1024];
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
 char uart_string[40];
uint16_t adc_max;
uint16_t adc_min;
uint16_t adc_average;
float vpp_volt;
float duty;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int counter=0;
int t=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  
	if(htim ->Instance==TIM3)
	{
		
		counter=__HAL_TIM_GetCounter(&htim2);
		counter +=t*65535;
		t=0;
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		
	}
	if(htim ->Instance==TIM2)
	{
		t++;
		
	}
}

unsigned int in_rise;
unsigned int in_fall;
unsigned char in_duty;
//unsigned int in_freq;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
			in_rise=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1)+1;
		
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
		{
			in_fall=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
			in_duty=in_fall*100/in_rise;

//			in_freq=1000000.0/in_rise;
		}
	}
	
}

void adc_proc()
{
//    static uint32_t mes_tick=0;
//    if(uwTick-mes_tick<1000)
//        return;
//    mes_tick = uwTick;
//    count++;
    
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_val,1024);
    HAL_Delay(20);
    uint32_t adc_sum=0;
    adc_max = 0;
    adc_min = 4096;
    adc_average = 0;
    uint16_t count1=0,count2=0;
    for(int i=0;i<1024;i++)
    {
//            sprintf(tmp,"   %d\r\n",adc_val[i]);
//            HAL_UART_Transmit(&huart1,(uint8_t *)tmp,strlen(tmp),100);
        if(adc_val[i]<adc_min)
        {
            adc_min = adc_val[i];
        }
        if(adc_val[i]>adc_max)
            adc_max = adc_val[i];
        if(adc_val[i]>(adc_max+adc_min)/2)
            count1++;
        else
            count2++;
        adc_sum += adc_val[i];
    }

    adc_average = adc_sum/1024;
    vpp_volt = (adc_max-adc_min)*3.3/4096;
    duty = count1*100.0/(count1+count2);

//		duty = (adc_average-adc_min)*100.0/(adc_max-adc_min);
//    sprintf(tmp,"   %d,%d,%d,%d\r\n",duty,adc_average,count1,count2);
//    HAL_UART_Transmit(&huart1,(uint8_t *)tmp,strlen(tmp),100);
    HAL_ADC_Stop_DMA(&hadc1);
}

void measure()
{
	static uint32_t mes_tick=0;
  if(uwTick-mes_tick<1000)
    return;
  mes_tick = uwTick;
		
	adc_proc();
		
	if(counter >=105000)
		vpp_volt =vpp_volt -0.1;
	
	if(counter >=3000)
	{
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_1);
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_2);
		HAL_TIM_Base_Stop(&htim4);
		return;
		
	}
	else
	{
		HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
		HAL_TIM_Base_Start(&htim4);
		duty=in_duty;
	}
	
}

void uart_proc()
{
	static uint32_t uart_uwtick;

	if(uwTick-uart_uwtick<1000) 
		return;
	uart_uwtick=uwTick;
	

	sprintf((char*)uart_string,"freq=%d \n",counter);
	HAL_UART_Transmit(&huart1,(uint8_t*)uart_string,strlen(uart_string),1000);
	
	sprintf((char*)uart_string,"vpp=%.2f \r\n",vpp_volt);
	HAL_UART_Transmit(&huart1,(uint8_t*)uart_string,strlen(uart_string),1000);
	
	sprintf((char*)uart_string,"duty=%.2f \r\n",duty);
	HAL_UART_Transmit(&huart1,(uint8_t*)uart_string,strlen(uart_string),1000);
	
	sprintf((char*)uart_string,"************** \r\n");
	HAL_UART_Transmit(&huart1,(uint8_t*)uart_string,strlen(uart_string),1000);
	

}



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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
	HAL_TIM_Base_Start(&htim4);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		measure();
		uart_proc();
		
    /* USER CODE END WHILE */

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
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
