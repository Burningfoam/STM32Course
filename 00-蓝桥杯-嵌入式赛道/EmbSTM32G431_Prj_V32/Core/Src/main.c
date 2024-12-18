/* USER CODE BEGIN Header */
/*******************************************************************************
* ��    �ߣ�hufq
* ע�����
* ������־��
* 20220920: 1�����LED������ʾ������ĵ�Ʒ�ʽ��
* 20220921: 1�����LCD������ʾ
* 20220922: 1�����button������ʾ
* 20220926: 1�����B4���ⲿ�жϹ�����ʾ
            ��1��ʹ�ð�����ʾ�ⲿ�жϣ����ж������⣻
            ��2�������Բ�Ӱ����������ɨ�裨��ʹ������B4�İ���ɨ�裩
* 20220928: 1����ɴ��ڹ��ܣ����ڽ������жϣ������ò�ѯ����δ��DMA��ʽ��
* 20220929: 1�����IIC���ܣ�����24C02��MCP4017������IICʱ������������Դ��������оƬ��������Ҫ�ο�оƬ��������׫д��
* 20221004: 1���������AD���ܣ���ͨ��(�ֱ���AD1��AD2)ʹ��DMA��ʽ���䣻
* 20221005: 1����ɻ�����ʱ��TIM6�Ĺ��ܣ�100ms�ж�һ�Σ�
* 20221007: 1����ɸ߼���ʱ��TIM2��TIM3�Ĳ����ܣ�����TIM2_CH1(PA15)��TIM3_CH1(PB4)�ⲿ���������Ƶ�ʺ�ռ�ձȣ�
* 20221008: 1����ɸ߼���ʱ��TIM4�ıȽ�ģʽ�������·��ͬ���ں�ռ�յ����壻
            2����ɸ߼���ʱ��TIM15��PWMģʽ�������·ͬ���ںͲ�ͬռ�յ����壻
* 20221010: 1�����DA���ܣ�DAC_CH1_PA4���ָ����ѹ��DAC_CH2_PA5�Զ�������ǲ�
* 20230407: 1�������˰����Ĵ�������ɨ����������ʽ���ڰ����������п������ְ��£���һ�μ�⣩���̰����������ɿ��⼸��״̬
            2�����ΪV30�汾
* �汾V31 : ��V30�汾�������޸�
            1�����Ӽ��޸Ĳ��ֺ���������˵��
            2��ȡ��B4���ⲿ�жϣ�B4��Ϊ��ͨ����ʹ��
            3����ʱ������TIM3�Ĳ������Ƶ�ʣ���ΪTIM2��TIM3ͬʱ���в����Ƶ�����R38��R39��ť������
               �������ź�Ƶ�����ʱ��33K���ң���������������Ͻ����жϣ����³����޷�����ִ�У�
* �汾V32 : ��V31�汾�������޸�
            1���޸���V31�汾�ϵ�3�������е����⣻ԭ�����ڼ���Ƶ�ʺ�ռ�ձ�ͬʱ���½����жϣ������ʱ�����µģ����԰�Ƶ�ʷ����������ж�ʱ����
               ����V32�в��������Ƶ��ж������ʾƵ�ʺ�ռ�ձ�ʱ�ټ��㣻
            2���޸�TIM3������������ź�ʱ����������ͨ����CH1(������)��CH2���½��أ���������һ��ͨ������һ��ͨ����ռ���޷�ʹ��ʱ�����в����������ڲ���������
               

*******************************************************************************/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "Bsp_Test.h"
#include "Bsp_LED.h"
#include "Bsp_Button.h"
#include "lcd.h"
#include "Bsp_Usart.h"
#include "i2c - hal.h"
#include "Bsp_AD.h"
#include "Bsp_DAC.h"
#include "Bsp_tim.h"
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM15_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
    LED_Control(LED_State);
    LCD_Init();
    LCD_TestTask();
//    BSP_LED_TestTask();
    //���Դ���
    sprintf((char *)Uart1_Tx_Buf, " hello world!20240124 V32r\n");  //ע����Ҫͷ�ļ���"stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
    
    I2CInit();
    IIC_Test_24C02();
    IIC_Test_MCP4017();
    
    Bsp_AD_Init();   
    
    /**********������ʱ��TIM6************/
    HAL_TIM_Base_Start_IT(&htim6);  //ʹ�ܶ�ʱ��6�Ͷ�ʱ��6�����жϣ�TIM_IT_UPDATE
    
    /**********��ʱ������ģʽTIM2��TIM3************/
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1); 
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2); 
    HAL_TIM_Base_Start(&htim2);//��ʱ��2���� ��ʹ��HAL_TIM_Base_Start_IT����Ч����ͬ�����Ǹú���ʹ����TIM_IT_UPDATE���������жϣ�    
    
    CaptureTim3_Start();
//    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1); 
//    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2); 
//    HAL_TIM_Base_Start(&htim3);//��ʱ��3���� ��ʹ��HAL_TIM_Base_Start_IT����Ч����ͬ�����Ǹú���ʹ����TIM_IT_UPDATE���������жϣ�    
    
    /**********��ʱ���Ƚ�ģʽTIM4************/
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1); //��ʱ��4�Ƚ����
	HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2); //��ʱ��4�Ƚ����
    
    /**********��ʱ��PWMģʽTIM15************/
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);   //��ʱ��15��PWN_CH1���
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_2);   //��ʱ��15��PWN_CH2���
	
    /**********DAC1_CH1_PA4���ģ���ѹ************/
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
    DAC1_CH1_Set_Vol(DAC1_CH1_Vol);
    DA_Display_Test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//        BSP_LED_TestTask();        
        KeyProcess();       //��������
        Uart1_RX_Pro();
        AD_Test();
        PulseMeasure_Test();
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
