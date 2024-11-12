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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "oled.h"
#include "i2c_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
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
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint16_t rxIndex = 0;
uint8_t rxData;  // ���ڽ��յ����ֽ�

uint8_t dataReceived = 0;

unsigned char Motor_flag = 0;
unsigned char percentRecv[3] = {0};
float threshold = 2.0;

char buffer[17] = {0};
char buffer2[17] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
	{
		threshold = (100 - atoi((char*)percentRecv)) / 100.0 * 3.3;
//		char buffer3[17] = {0};
//		snprintf(buffer3,sizeof(buffer3),"now:   %.2fV \n",threshold);
////		HAL_UART_Transmit(&huart1,(uint8_t *)buffer,sizeof(buffer),1000);
//		HAL_UART_Transmit(&huart1,(uint8_t *)buffer3,sizeof(buffer3),1000);
		HAL_UART_Receive_IT(&huart1,percentRecv,2);
	}
	if(huart->Instance == USART2)
	{
        // �����յ����ֽڷ�����ջ�����
        if (rxIndex < RX_BUFFER_SIZE - 1)
        {
            rxBuffer[rxIndex++] = rxData;
					
            // ������յ����з���ﵽ���������ޣ���Ϊһ�����ݰ�����
            if (rxData == '\n' || rxIndex == RX_BUFFER_SIZE - 1)
            {
                rxBuffer[rxIndex] = '\0';  // ����ַ���������
                dataReceived = 1;
                rxIndex = 0;  // ���������Ա���һ�ν���
            }
        }
        else
        {
            // ��ֹ���
            rxIndex = 0;
        }

        // ���������жϽ���
        HAL_UART_Receive_IT(&huart2, &rxData, 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim2)
	{
		OLED_ShowString(0,0, (uint8_t *)buffer, 15); 
		OLED_ShowString(0,10, (uint8_t *)buffer2, 15); 
	}
}

void UART_Printf(const char *format, ...)	// ���ڵ��Դ�ӡ����
{
    uint8_t tmp[128];
     
    va_list argptr;
    va_start(argptr, format);

    // ʹ�� vsnprintf �����������
    vsnprintf((char *)tmp, sizeof(tmp), format, argptr);

    va_end(argptr);

    // ��������
    HAL_UART_Transmit(&huart1, tmp, strlen((char *)tmp), HAL_MAX_DELAY);
}

// ����ATָ��ȴ���Ӧ, ����������ʽ���WiFiģ��ĳ�ʼ���Ͳ��Թ���
uint8_t ESP_SendATCommand(char* cmd, char* expected_response, uint32_t timeout) 
{
    char response[256];
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), timeout);  // ����ATָ��
    HAL_UART_Receive(&huart2, (uint8_t*)response, sizeof(response) - 1, timeout);  // ������Ӧ

    if (strstr(response, expected_response) != NULL) 
	{
        return 1;  // �ɹ�
    } 
	else 
	{
        return 0;  // ʧ��
    }
}


/* ����WiFi���� */
uint8_t ESP_ConnectWiFi(char* ssid, char* password)
{
    char cmd[256];
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);  // ��������WiFi��ATָ��
    return ESP_SendATCommand(cmd, "OK", MAX_TIMEOUT);  // ��������ȴ���Ӧ
}

/* ����TCP���Ӻ��� */
uint8_t ESP_ConnectServer(char* ip, int port)
{
    char cmd[256];
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);  // ����TCP���ӵ�ATָ��
    return ESP_SendATCommand(cmd, "CONNECT", TIMEOUT);  // ��������ȴ���Ӧ
}


/* �������ݺ��� */
/**
	����������ȷ��� `AT+CIPSEND` ����֪ESP-01SҪ���͵����ݳ��ȣ�Ȼ���ٷ���ʵ�ʵ��������ݡ������ESP-01S�Ƿ񷵻�`SEND OK`��ʾ���ݷ��ͳɹ���
*/
uint8_t ESP_SendData(char* data)
{
    char cmd[64];
    sprintf(cmd, "AT+CIPSEND=%d\r\n", strlen(data));  // ���ɷ������ݳ��ȵ�ATָ��
    if (!ESP_SendATCommand(cmd, ">", TIMEOUT)) 	// ����Ƿ�׼���÷�������
		{  
        return 0;
		
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), TIMEOUT);  // ����ʵ������
    return ESP_SendATCommand("", "SEND OK", TIMEOUT);  // ����Ƿ��ͳɹ�
		
}

void ESP_SendData_CIPMODE_1(char *data)
{
	// �ر��жϽ��գ���Ϊ����ģʽ
    HAL_UART_AbortReceive_IT(&huart2);  
	
	HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), TIMEOUT);  // ����ʵ������
	
	// ���¿����жϽ���
    HAL_UART_Receive_IT(&huart2, &rxData, 1);
}

uint8_t ESP_ReceiveData_CIPMODE_1(char *buffer, int buffer_size)
{
	char response[256] = { 0 }; // ��ʱ�洢���յ��ַ�
    int index = 0;              // �ַ���������
    HAL_StatusTypeDef status;
    uint8_t received_byte;
	
	// ѭ����ȡÿ���ֽڣ�ֱ��������������ﵽ����������
    while (index < buffer_size - 1)
    {
        // ����һ���ֽ�����
        status = HAL_UART_Receive(&huart2, &received_byte, 1, 1000);
        
        if (status == HAL_OK)
        {
            // �����յ����ֽڴ�����Ӧ����
            response[index++] = received_byte;

            // ����Ƿ�Ϊ�����������������Ի��з� '\n' ������
            if (received_byte == '\n')
            {
                break;
            }
        }
        else
        {
            // ���ճ�ʱ��ʧ�ܣ���ӡ������Ϣ������0
            UART_Printf("Failed to receive data\n");
            return 0;
        }
    }

    // ����ַ���������
    response[index] = '\0';

    // ��ӡ������Ϣ�������յ���������Ӧ
    UART_Printf("Response: %s\n", response);
	
	return 1;
}

/* �������ݺ��� */
void ESP_ReceiveData(char* buffer, int buffer_size) 
{
//	HAL_UART_Receive_IT(&huart1, (uint8_t *)g_response, sizeof(g_response));
}

/* �ر�TCP���Ӻ��� */
uint8_t ESP_CloseConnection() 
{
    return ESP_SendATCommand("AT+CIPCLOSE\r\n", "CLOSED", TIMEOUT);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MotorStart(float volt)
{
	// ʹ�þ�̬�����洢��һ�ε��õ�ʱ��
	static uint32_t LastStartTime = 0;
	uint32_t CurrentTime = HAL_GetTick();

	if(Motor_flag == 0)
	{
		if(CurrentTime > LastStartTime + 3000 && CurrentTime < LastStartTime + 10000)
			return;
		else if(CurrentTime > LastStartTime + 10000 && volt < threshold)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
			LastStartTime = CurrentTime;
			Motor_flag = 1;
			return;
		}
		else 
			return;
	}
	else if(Motor_flag == 1)
	{
		if(CurrentTime < LastStartTime + 3000 )
			return;
		else if(CurrentTime > LastStartTime + 3000 )
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
			Motor_flag = 0;
			return;
		}
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
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_ADC3_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	/* ����WiFiģ�� */
	// 1. ����ESP8266ΪSTA + AP ģʽ
  if (ESP_SendATCommand("AT+CWMODE=1\r\n", "OK", TIMEOUT) == 0)
	{
		UART_Printf("WiFi AT+CWMODE=1 Failed!\n");
	}
		
	// ����Ϊ��͸��ģʽ
	if (ESP_SendATCommand("AT+CIPMODE=0\r\n", "OK", TIMEOUT) == 0)
	{
		UART_Printf("Failed to set non-transparent mode\n");
	}
	
	// 2. ����WiFi
    if (ESP_ConnectWiFi("Redmi K70", "ac12234567"))	// ���Կ����ȵ����
	{
        UART_Printf("WiFi connected\n");
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);	// ���̵�, ��ʾWiFiģ��ɹ���������
    }
	else 
	{
        UART_Printf("WiFi connection failed\n");
    }
		
	// 3. ���ӷ�����
    if (ESP_ConnectServer("121.40.108.31", 8080)) 	// ����
	{
        UART_Printf("Connected to server\n");
    } 
	else 
	{
        UART_Printf("Server connection failed\n");
    }
		
	// 4. ���Ͳ������ݵ�������
    if (ESP_SendData("hello!!! I am ESP-01S")) 
	{
        UART_Printf("'hello' sent successfully\n");
    } 
	else 
	{
        UART_Printf("'hello' send failed\n");
    }
//		if (ESP_SendData("hello!!! I am ESP-01S")) 
//		{
//				UART_Printf("'hello' sent successfully\n");
//		} 
//		else 
//		{
//				UART_Printf("'hello' send failed after retries\n");
//		}

	
	//�Ƿ���͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
	if (ESP_SendATCommand("AT+CIPMODE=1\r\n", "OK", TIMEOUT))
	{
		UART_Printf("AT+CIPMODE=1 OK\n");
	}
	else
	{
		UART_Printf("AT+CIPMODE=1 Failed\n");
	}
	
	//͸��ģʽ�� ��ʼ�������ݵ�ָ�� ���ָ��֮��Ϳ���ֱ�ӷ�������
	if (ESP_SendATCommand("AT+CIPSEND\r\n", "OK", TIMEOUT))
	{
		UART_Printf("AT+CIPSEND, OK\n");
	}
	else
	{
		UART_Printf("AT+CIPSEND, Failed\n");
	}
	
	
  IIC_Init();
  OLED_Init();

  OLED_Clear();
	
//	OLED_ShowString(0,0, (uint8_t *)"Hello World!", 15);
//	OLED_ShowString(0,10, (uint8_t *)"Festival Happy!", 15);
//	OLED_ShowString(0,12, (uint8_t *)"Weekend Happy!", 15);
//	OLED_ShowString(0,14, (uint8_t *)"Week Happy!", 15);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_UART_Receive_IT(&huart1,percentRecv,2);
	HAL_TIM_Base_Start_IT(&htim2);
  while (1)
  {
		//����ADC��������
		HAL_ADC_Start(&hadc3);
		//��ѯ�ȴ���������ת�����
		HAL_ADC_PollForConversion(&hadc3,HAL_MAX_DELAY);
		
		//��ȡ��������ת�����
		uint32_t result = HAL_ADC_GetValue(&hadc3);
		//result ��Χ��0 - 4096 ���ö�Ӧ��ѹ��0-3.3V 
		float volt = result/4096.0 * 3.3;
		int percent =100 -  volt/3.3 * 100.0;
		
		//char buffer[17] = {0};
		snprintf(buffer,sizeof(buffer),"result:%.2fV ",volt);
		//OLED_ShowString(0,0, (uint8_t *)buffer, 15); 
		//char buffer2[17] = {0};
		snprintf(buffer2,sizeof(buffer2),"       %d%%     \n",percent);
		//OLED_ShowString(0,10, (uint8_t *)buffer2, 15); 
		HAL_UART_Transmit(&huart1,(uint8_t *)buffer,sizeof(buffer),1000);
		char buffer3[17] = {0};
		snprintf(buffer3,sizeof(buffer3),"threshold:%.2fV ",threshold);
		HAL_UART_Transmit(&huart1,(uint8_t *)buffer3,sizeof(buffer3),1000);
		HAL_UART_Transmit(&huart1,(uint8_t *)buffer2,sizeof(buffer2),1000);
		MotorStart(volt);
//		HAL_Delay(1000);
		
//		if(volt<2)
//			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
//		else
//			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		ESP_SendData_CIPMODE_1(strcat(buffer,buffer2));
		HAL_Delay(1000);
		if (dataReceived)
		{
			dataReceived = 0;
			UART_Printf((char *)rxBuffer);
		}
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
