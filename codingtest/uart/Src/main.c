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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

u16 t1=0,t2,t=0;               // 定时器寄存器为16位
u32 b1,b2;
u32 i;
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

u32 USART1_Baud(void)                                                                        
{
	HAL_TIM_Base_Start(&htim5);
	b1 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);            // 读GPIOA.10的电平
	for(i=0;i<32;)                                    // 连续检测GPIO.10引脚32次电平变更
	{
		
			b2 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);    // 读GPIOA.10的新值
			
			if(b2 != b1)                               // 如果有电平变更
			{
				t2 = __HAL_TIM_GET_COUNTER(&htim5);         // 读定时器中的值
				b1 = b2;                          // 更新为新的引脚值
				if((t1 == 0)&&(t==0))        // 第一个电平变更
				{
					t1 = t2;                     // 记录第一个时刻点
				}
				else                                  // 不是第一个电平变更
				{
					if(t == 0)                   // 第一段电平
					{
						t = t2-t1;           // 记录第一段电平所用时间
					}
					else                          // 不是第一段电平
					{
						if((t2-t1)< t)       // 保存电平段的最小值 
						{
							t = t2-t1;
						}
					}
					t1 = t2;                      // 更新为新的时刻点
				}
				i++;                                  // 电平变更数+1
			}
	}
	HAL_TIM_Base_Stop(&htim5);                               // 关闭TIM5的时钟
	__HAL_TIM_SetCounter(&htim5,0);
	return ((u32)t*403/400);  
}

u8 rx_tmp;
u8 rx_buf[20];
u8 rx_len;
u8 flag_baud,flag_rx;


void check_baud()
{
	if(flag_baud)
	{
		u32 bps = 0;
		u8 val = USART1_Baud();
		if(abs(val - 105) < 3)	bps = 9600;
		else if(abs(val - 53) < 3)	bps = 19200;
		else if(abs(val - 13) < 3)	bps = 76800;
		else if(abs(val - 8) < 3)	bps = 115200;
		
		if(bps != 0)
		{
			flag_rx = 1;
			huart1.Init.BaudRate = bps;
			HAL_UART_Init(&huart1);
			HAL_UART_AbortReceive_IT(&huart1);
			
			printf("%d\r\n",bps);
//			HAL_TIM_Base_Stop_IT(&htim7);
			HAL_UART_Receive_IT(&huart1,(u8 *)&rx_tmp,1);
			memset(rx_buf,0,rx_len);
			rx_len = 0;
		}
	}
}

__IO u32 uwtick_rx;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(!flag_rx)	
	{
		HAL_TIM_Base_Start_IT(&htim7);
		flag_baud = 1;
	}
	else	flag_baud = 0;
	uwtick_rx = uwTick;
	rx_buf[rx_len++] = rx_tmp;
	HAL_UART_Receive_IT(&huart1,(u8 *)&rx_tmp,1);
}

void rx_proc()
{
	if(uwTick - uwtick_rx < 10)	return;
	uwtick_rx = uwTick;
	
	if(rx_len)
	{
		if(flag_rx)
		{
			HAL_TIM_Base_Stop_IT(&htim7);
			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
			printf("%s\r\n",rx_buf);
			flag_rx = 0;
		}
		memset(rx_buf,0,rx_len);
		rx_len = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	}
	if(htim->Instance == TIM7)
	{
		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
	}
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
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_UART_Receive_IT(&huart1,(u8 *)&rx_tmp,1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		check_baud();
		rx_proc();
		
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
