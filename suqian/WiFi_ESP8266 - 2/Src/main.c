/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_TIMEOUT 10000
#define TIMEOUT 500 //200
#define RX_BUFFER_SIZE 512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* 全局变量 */
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint16_t rxIndex = 0;
uint8_t rxData;  // 用于接收单个字节

uint8_t dataReceived = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void UART_Printf(const char *format, ...)	// 串口调试打印函数
{
    uint8_t tmp[128];
     
    va_list argptr;
    va_start(argptr, format);

    // 使用 vsnprintf 限制输出长度
    vsnprintf((char *)tmp, sizeof(tmp), format, argptr);

    va_end(argptr);

    // 发送数据
    HAL_UART_Transmit(&huart1, tmp, strlen((char *)tmp), HAL_MAX_DELAY);
}

// 发送AT指令并等待响应, 阻塞函数方式完成WiFi模块的初始化和测试工作
uint8_t ESP_SendATCommand(char* cmd, char* expected_response, uint32_t timeout) 
{
    char response[256];
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), timeout);  // 发送AT指令
    HAL_UART_Receive(&huart2, (uint8_t*)response, sizeof(response) - 1, timeout);  // 接收响应

    if (strstr(response, expected_response) != NULL) 
	{
        return 1;  // 成功
    } 
	else 
	{
        return 0;  // 失败
    }
}

/* 连接WiFi函数 */
uint8_t ESP_ConnectWiFi(char* ssid, char* password)
{
    char cmd[256];
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);  // 生成连接WiFi的AT指令
    return ESP_SendATCommand(cmd, "OK", MAX_TIMEOUT);  // 发送命令并等待响应
}

/* 建立TCP连接函数 */
uint8_t ESP_ConnectServer(char* ip, int port)
{
    char cmd[256];
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);  // 生成TCP连接的AT指令
    return ESP_SendATCommand(cmd, "CONNECT", TIMEOUT);  // 发送命令并等待响应
}

/* 发送数据函数 */
/**
	这个函数首先发送 `AT+CIPSEND` 来告知ESP-01S要发送的数据长度，然后再发送实际的数据内容。最后检查ESP-01S是否返回`SEND OK`表示数据发送成功。
*/
uint8_t ESP_SendData(char* data)
{
    char cmd[64];
    sprintf(cmd, "AT+CIPSEND=%d\r\n", strlen(data));  // 生成发送数据长度的AT指令
    if (!ESP_SendATCommand(cmd, ">", TIMEOUT)) 	// 检查是否准备好发送数据
	{  
        return 0;
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), TIMEOUT);  // 发送实际数据
    return ESP_SendATCommand("", "SEND OK", TIMEOUT);  // 检查是否发送成功
}

void ESP_SendData_CIPMODE_1(char *data)
{
	// 关闭中断接收，改为阻塞模式
    HAL_UART_AbortReceive_IT(&huart2);  
	
	HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), TIMEOUT);  // 发送实际数据
	
	// 重新开启中断接收
    HAL_UART_Receive_IT(&huart2, &rxData, 1);
}

uint8_t ESP_ReceiveData_CIPMODE_1(char *buffer, int buffer_size)
{
	char response[256] = { 0 }; // 临时存储接收的字符
    int index = 0;              // 字符数组索引
    HAL_StatusTypeDef status;
    uint8_t received_byte;
	
	// 循环读取每个字节，直到遇到结束符或达到缓冲区限制
    while (index < buffer_size - 1)
    {
        // 接收一个字节数据
        status = HAL_UART_Receive(&huart2, &received_byte, 1, 1000);
        
        if (status == HAL_OK)
        {
            // 将接收到的字节存入响应数组
            response[index++] = received_byte;

            // 检查是否为结束符（假设数据以换行符 '\n' 结束）
            if (received_byte == '\n')
            {
                break;
            }
        }
        else
        {
            // 接收超时或失败，打印调试信息并返回0
            UART_Printf("Failed to receive data\n");
            return 0;
        }
    }

    // 添加字符串结束符
    response[index] = '\0';

    // 打印调试信息，检查接收到的完整响应
    UART_Printf("Response: %s\n", response);
	
	return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)	// 串口中断 响应函数
{
	if(huart->Instance == USART2)
	{
        // 将接收到的字节放入接收缓冲区
        if (rxIndex < RX_BUFFER_SIZE - 1)
        {
            rxBuffer[rxIndex++] = rxData;
					
            // 如果接收到换行符或达到缓冲区上限，认为一个数据包完整
            if (rxData == '\n' || rxIndex == RX_BUFFER_SIZE - 1)
            {
                rxBuffer[rxIndex] = '\0';  // 添加字符串结束符
                dataReceived = 1;
                rxIndex = 0;  // 重置索引以便下一次接收
            }
        }
        else
        {
            // 防止溢出
            rxIndex = 0;
        }

        // 重新启动中断接收
        HAL_UART_Receive_IT(&huart2, &rxData, 1);
	}
}

/* 接受数据函数 */
void ESP_ReceiveData(char* buffer, int buffer_size) 
{
//	HAL_UART_Receive_IT(&huart1, (uint8_t *)g_response, sizeof(g_response));
}

/* 关闭TCP连接函数 */
uint8_t ESP_CloseConnection() 
{
    return ESP_SendATCommand("AT+CIPCLOSE\r\n", "CLOSED", TIMEOUT);
}
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  
	/* 测试WiFi模块 */
	// 1. 设置ESP8266为STA + AP 模式
    if (ESP_SendATCommand("AT+CWMODE=1\r\n", "OK", TIMEOUT) == 0)
	{
		UART_Printf("WiFi AT+CWMODE=1 Failed!\n");
	}
		
	// 设置为非透传模式
	if (ESP_SendATCommand("AT+CIPMODE=0\r\n", "OK", TIMEOUT) == 0)
	{
		UART_Printf("Failed to set non-transparent mode\n");
	}
	
	// 2. 连接WiFi
    if (ESP_ConnectWiFi("Redmi K70", "ac12234567"))	// 电脑开启热点分享
	{
        UART_Printf("WiFi connected\n");
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);	// 亮绿灯, 表示WiFi模块成功连接网络
    }
	else 
	{
        UART_Printf("WiFi connection failed\n");
    }
		
	// 3. 连接服务器
    if (ESP_ConnectServer("121.40.108.31", 8080)) 	// 连接
	{
        UART_Printf("Connected to server\n");
    } 
	else 
	{
        UART_Printf("Server connection failed\n");
    }
		
	// 4. 发送测试数据到服务器
    if (ESP_SendData("hello!!! I am ESP-01S")) 
	{
        UART_Printf("'hello' sent successfully\n");
    } 
	else 
	{
        UART_Printf("'hello' send failed\n");
    }
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	if (ESP_SendATCommand("AT+CIPMODE=1\r\n", "OK", TIMEOUT))
	{
		UART_Printf("AT+CIPMODE=1 OK");
	}
	else
	{
		UART_Printf("AT+CIPMODE=1 Failed");
	}
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	if (ESP_SendATCommand("AT+CIPSEND\r\n", "OK", TIMEOUT))
	{
		UART_Printf("AT+CIPSEND, OK");
	}
	else
	{
		UART_Printf("AT+CIPSEND, Failed");
	}
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
//	  	if (!ESP_SendData("Are you OK?"))	// 发送采集数据信息给服务器 (非透传模式)
//		{
//			UART_Printf("Data send Failed!\n");
//		}
//		
		ESP_SendData_CIPMODE_1("Are you OK?");
		HAL_Delay(1000);
		if (dataReceived)
		{
			dataReceived = 0;
			UART_Printf((char *)rxBuffer);
		}
		
//		ESP_ReceiveData(buffer, sizeof(buffer));

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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
