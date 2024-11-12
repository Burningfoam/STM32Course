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
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMEOUT 15000 // 超时时间
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

// 发送AT指令并等待响应
int ESP_SendATCommand(char* cmd, char* expected_response, uint32_t timeout)
{
	char response[256];
	HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), timeout); // 发送AT指令
	HAL_UART_Receive(&huart2, (uint8_t*)response, sizeof(response)-1, timeout);
	// 接收响应
	if (strstr(response, expected_response) != NULL) 
	{
		return 1; // 成功
	} 
	else 
	{
		return 0; // 失败
	}
}

int ESP_ConnectWiFi(char* ssid, char* password) 
{
	char cmd[256];
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password); // 生成连接WiFi的AT指令
	return ESP_SendATCommand(cmd, "OK", TIMEOUT); // 发送命令并等待响应
}

int ESP_ConnectServer(char* ip, int port) 
{
	char cmd[256];
	sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port); // 生成TCP连接的AT指令
	return ESP_SendATCommand(cmd, "CONNECT", TIMEOUT); // 发送命令并等待响应
}

int ESP_SendData(char* data) 
{
	char cmd[256];
	sprintf(cmd, "AT+CIPSEND=%d\r\n", strlen(data)); // 生成发送数据长度的AT指令
	if (!ESP_SendATCommand(cmd, ">", TIMEOUT)) 
	{ // 检查是否准备好发送数据
		return 0;
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), TIMEOUT); // 发送实际数据
	return ESP_SendATCommand("", "SEND OK", TIMEOUT); // 检查是否发送成功
}

int ESP_ReceiveData(char* buffer, int buffer_size) 
{
	char response[256] = {0};
	HAL_UART_Receive(&huart2, (uint8_t*)response, sizeof(response)-1, TIMEOUT);
	char * pid_pos =strstr(response,"+IPD");
	if(pid_pos != NULL)
	{
		char* data_start=strchr(pid_pos,':');
		if(data_start != NULL)
		{
			/*移动到实际数据开始的地方 */
			data_start++;
			
			strncpy(buffer,data_start,buffer_size-1);
			/*字符串结東符 */
			buffer[buffer_size-1]='\0';
			return 1;
		}
	}
	return 0;
	// 从串口接收数据
}

int ESP_CloseConnection() 
{
	return ESP_SendATCommand("AT+CIPCLOSE\r\n", "CLOSED", TIMEOUT);
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
//	// 1. 设置WIFI模式
//	ESP_SendATCommand("AT+CWMODE=3\r\n", "OK", TIMEOUT);
//	// 2. 连接WiFi
//	if (ESP_ConnectWiFi("Redmi K70", "ac12234567")) 
//	{ 
//		printf("WiFi connected\n");
//	} 
//	else 
//	{
//		printf("WiFi connection failed\n");
//	}
//	// 3. 连接服务器
//	if (ESP_ConnectServer("121.40.108.31", 8080)) 
//	{ 
//		printf("Connected to server\n");
//	} 
//	else 
//	{
//		printf("Server connection failed\n");
//	}
//	// 4. 发送数据到服务器
//	if (ESP_SendData("Hello")) 
//	{ 
//		printf("Data sent successfully\n");
//	} 
//	else 
//	{
//		printf("Data send failed\n");
//	}
//	
//	// 5. 接收服务器返回的数据
//	char buffer[256];
//	if (ESP_ReceiveData(buffer, sizeof(buffer))) 
//	{ 
//		printf("Received: %s\n", buffer);
//	}
	// 6. 关闭连接
//	ESP_CloseConnection();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	ESP_SendATCommand("AT+CWMODE=1\r\n", "OK", TIMEOUT);
	// 2. 连接WiFi
	ESP_ConnectWiFi("Redmi K70", "ac12234567");
	
	// 3. 连接服务器
	ESP_ConnectServer("121.40.108.31", 8080);
	char recvBuffer[256] = {0};
  while (1)
  {
	// 4. 发送数据到服务器
	ESP_SendData("Hello");
	
	HAL_Delay(5000);
		
	ESP_ReceiveData(recvBuffer,sizeof(recvBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)recvBuffer, sizeof (recvBuffer), HAL_MAX_DELAY);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
