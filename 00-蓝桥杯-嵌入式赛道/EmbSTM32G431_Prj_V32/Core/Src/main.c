/* USER CODE BEGIN Header */
/*******************************************************************************
* 作    者：hufq
* 注意事项：
* 进度日志：
* 20220920: 1、完成LED功能演示，后更改点灯方式；
* 20220921: 1、完成LCD功能演示
* 20220922: 1、完成button功能演示
* 20220926: 1、完成B4的外部中断功能演示
            （1）使用按键演示外部中断，会有抖动问题；
            （2）经测试不影响正常按键扫描（即使包含了B4的按键扫描）
* 20220928: 1、完成串口功能：串口接收用中断，发送用查询；（未用DMA方式）
* 20220929: 1、完成IIC功能：包括24C02和MCP4017，其中IIC时序驱动拷贝资源包，两个芯片的驱动需要参考芯片资料自行撰写；
* 20221004: 1、完成内置AD功能，俩通道(分别是AD1和AD2)使用DMA方式传输；
* 20221005: 1、完成基本定时器TIM6的功能，100ms中断一次；
* 20221007: 1、完成高级定时器TIM2，TIM3的捕获功能，测量TIM2_CH1(PA15)和TIM3_CH1(PB4)外部输入脉冲的频率和占空比；
* 20221008: 1、完成高级定时器TIM4的比较模式，输出多路不同周期和占空的脉冲；
            2、完成高级定时器TIM15的PWM模式，输出多路同周期和不同占空的脉冲；
* 20221010: 1、完成DA功能；DAC_CH1_PA4输出指定电压，DAC_CH2_PA5自动输出三角波
* 20230407: 1、改善了按键的处理，按键扫描用连续方式，在按键处理函数中可以区分按下（第一次检测），短按，长按，松开这几种状态
            2、后改为V30版本
* 版本V31 : 在V30版本基础上修改
            1、增加及修改部分函数的文字说明
            2、取消B4的外部中断，B4作为普通按键使用
            3、暂时屏蔽了TIM3的捕获测量频率，因为TIM2和TIM3同时进行捕获测频，如果R38、R39旋钮到最左
               即被测信号频率最大时（33K左右），程序会连续不断进入中断，导致程序无法正常执行；
* 版本V32 : 在V31版本基础上修改
            1、修改了V31版本上第3点描述中的问题；原因是在计算频率和占空比同时在下降沿中断，计算耗时长导致的，可以把频率放在上升沿中断时计算
               或者V32中采用整体移到中断外的显示频率和占空比时再计算；
            2、修改TIM3捕获测量脉冲信号时，用了两个通道（CH1(上升沿)和CH2（下降沿）），改用一个通过（在一个通道被占用无法使用时）进行测量（常用于测量脉宽）；
               

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
    //测试串口
    sprintf((char *)Uart1_Tx_Buf, " hello world!20240124 V32r\n");  //注意需要头文件："stdio.h"
    Uart1_Send(Uart1_Tx_Buf,strlen((char *)Uart1_Tx_Buf));
    
    I2CInit();
    IIC_Test_24C02();
    IIC_Test_MCP4017();
    
    Bsp_AD_Init();   
    
    /**********基本定时器TIM6************/
    HAL_TIM_Base_Start_IT(&htim6);  //使能定时器6和定时器6更新中断：TIM_IT_UPDATE
    
    /**********定时器捕获模式TIM2和TIM3************/
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1); 
    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2); 
    HAL_TIM_Base_Start(&htim2);//定时器2启动 ，使用HAL_TIM_Base_Start_IT函数效果相同（但是该函数使能了TIM_IT_UPDATE，即更新中断）    
    
    CaptureTim3_Start();
//    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1); 
//    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2); 
//    HAL_TIM_Base_Start(&htim3);//定时器3启动 ，使用HAL_TIM_Base_Start_IT函数效果相同（但是该函数使能了TIM_IT_UPDATE，即更新中断）    
    
    /**********定时器比较模式TIM4************/
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1); //定时器4比较输出
	HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2); //定时器4比较输出
    
    /**********定时器PWM模式TIM15************/
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);   //定时器15的PWN_CH1输出
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_2);   //定时器15的PWN_CH2输出
	
    /**********DAC1_CH1_PA4输出模拟电压************/
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
        KeyProcess();       //按键处理
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
