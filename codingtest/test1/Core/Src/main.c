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
#include "string.h"
#include "stdio.h"
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

unsigned int baud;
unsigned int t1=0,t2=0,t=0;               // ��ʱ���Ĵ���Ϊ16λ
unsigned int b2=1;
unsigned int b1=1;
unsigned int i;

char a[20];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART1)
	 {
	    uart_rx_string[rx_count++] = uart_rx_temp;
	    if(rx_count == 49)
				rx_count = 0;
	    HAL_UART_Receive_IT(&huart1,&uart_rx_temp,1);
			sprintf(a,"%c",uart_rx_temp);
			HAL_UART_Transmit(&huart1,(uint8_t *)a,1,1000);
	 }
}
unsigned char flag2=0;
void baud_test()
{
	static uint32_t baud_uwtick;
	
	if(uwTick - baud_uwtick < 1000)
	  return;
	baud_uwtick = uwTick;
	
	flag2 = (flag2+1)%5;
	if(flag2==0)
	{
		if(t>=8 && t<=10)
		{
			baud = 115200;
			huart1.Init.BaudRate = baud;
		}
	else if(t>=12 && t<=15)
	{
			baud = 76800;
		huart1.Init.BaudRate = baud;
	}
	else if(t>=51 && t<=53)
	{
			baud = 19200;
		huart1.Init.BaudRate = baud;
	}
	else if(t>=102 && t<=105)
	{
			baud = 9600;
		huart1.Init.BaudRate = baud;
	}
	// ����������ֵ(���ϵ�ƽ�����б�ʣ����Ϊ0.75%������ֵ)
//	huart1.Init.BaudRate = baud;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	b1 = 1;
	b2 = 1;
	t1 = 0;
	t2 = 0;
	
//	b1=0;
//	b2=0;

	}
//	baud=((unsigned int)t*403/400);  
//	// ����������ֵ(���ϵ�ƽ�����б�ʣ����Ϊ0.75%������ֵ)
//	huart1.Init.BaudRate = baud;
//	if (HAL_UART_Init(&huart1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	b1=0;
//	b2=0;
//	baud_uwtick = uwTick;
}


void led_proc()
{
	static unsigned char flag=0;
	static uint32_t led_uwtick;
	if(uwTick - led_uwtick < 50)
	  return;
	
	led_uwtick = uwTick;
	
	if(flag==0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
		flag=1;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
		flag=0;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 if(flag2 == 4)
		 {
				b1 = HAL_GPIO_ReadPin(GPIOA,10);            // ��GPIOA.10�ĵ�ƽ
				if(b2 != b1)                               // ����е�ƽ���
				{
					t2 = __HAL_TIM_GetCounter(&htim2);         // ����ʱ���е�ֵ
				b1 = b2;                          // ����Ϊ�µ�����ֵ
				if((t1 == 0)&&(t==0))        // ��һ����ƽ���
				{
								t1 = t2;                     // ��¼��һ��ʱ�̵�
				}
				else                                  // ���ǵ�һ����ƽ���
				{
								if(t == 0)                   // ��һ�ε�ƽ
								{
												t = t2-t1;           // ��¼��һ�ε�ƽ����ʱ��
								}
								else                          // ���ǵ�һ�ε�ƽ
								{
												if((t2-t1)< t)       // �����ƽ�ε���Сֵ 
												{
																t = t2-t1;
												}
								}
								t1 = t2;                      // ����Ϊ�µ�ʱ�̵�
				}
				}
		 }
		 
		baud_test();

//	  Uart1_RX();
//		led_proc();
				
	
				
  /* USER CODE END 3 */

	}
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
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
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
