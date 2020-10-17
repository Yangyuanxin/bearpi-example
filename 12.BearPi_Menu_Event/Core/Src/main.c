/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "fatfs.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*定时器设置为100ms刷新一次*/
#define TIMER_TIMEOUT 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Timer DataTime_Timer ;
Timer Key_Timer ;
Timer Test_Timer ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void DataTime_Timer_CallBack(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*按键扫描*/
void Key_CallBack()
{
    __IO uint8_t KeyCode = 255;
    //获取键值
    GetKey(&KeyCode);

    if(255 != KeyCode)
    {
        Menu_Select_Item(Flow_Cursor.flow_cursor, KeyCode);
        KeyCode = 255 ;
    }
}

/*RTC实时时钟显示*/
void DataTime_Timer_CallBack(void)
{
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
            DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day, \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
           );
    LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24);
}

/*测试回调*/
void Test_CallBack(void)
{
    static uint8_t Count_AMI = 0;
    static uint8_t Refresh_flag = 0 ;
    int smoke_value = 0 ;

    if(Flow_Cursor.flow_cursor == TEST_PAGE && detect_logic.Start_Detect == 1)
    {
        switch(detect_logic.Detect_Step)
        {
            case BASE_LINE:
                Count_AMI++ ;

                if(Count_AMI >= 3)
                    Count_AMI = 0 ;

                icon_reflash(Count_AMI);
                smoke_value = mq2_sensor_interface.get_smoke_value(&mq2_sensor_interface) ;

                if(smoke_value < ALARM_THRESHOLD / 2)
                {
                    display_smoke_value(smoke_value, GREEN, 1);
                    ++detect_logic.Count_Base ;
                }
                else
                {
                    display_smoke_value(smoke_value, RED, 1);
                }

                if(detect_logic.Count_Base > 10)
                {
                    detect_logic.Count_Base = 0 ;
                    /*隐藏基准*/
                    display_base(0);
                    /*显示检测*/
                    display_detect(1);
                    /*显示进度条框*/
                    Display_Process_Bar_Frame(1);
                    /*切换到检测中*/
                    detect_logic.Detect_Step = DETECTING ;
                    break ;
                }

                break ;

            case DETECTING:
                Count_AMI++ ;

                if(Count_AMI >= 3)
                    Count_AMI = 0 ;

                icon_reflash(Count_AMI);
                ++detect_logic.Test_Process ;

                /*测试安全*/
                if(detect_logic.Test_Process == 100 && mq2_sensor_interface.Smoke_Value < ALARM_THRESHOLD)
                {
                    detect_logic.Detect_Step = DETECT_SAFETY ;
                    Display_Process_Bar(0, 0);
                    display_smoke_value(smoke_value, BLACK, 0);
                    /*隐藏检测*/
                    display_detect(0);
                    /*显示安全*/
                    display_safety(1);
                    break ;
                }
                else
                {
                    smoke_value = mq2_sensor_interface.get_smoke_value(&mq2_sensor_interface) ;

                    //printf("检测:%d\n",smoke_value);
                    if(mq2_sensor_interface.Smoke_Value < ALARM_THRESHOLD)
                        display_smoke_value(smoke_value, GREEN, 1);
                    else
                    {
                        display_smoke_value(smoke_value, RED, 1);
                        detect_logic.Count_Alarm++ ;

                        if(detect_logic.Count_Alarm > 20)
                        {
                            detect_logic.Detect_Step = DETECT_DANGER ;
                            detect_logic.Count_Alarm = 0 ;
                            display_smoke_value(smoke_value, BLACK, 0);
                            Display_Process_Bar(0, 0);
                            /*隐藏检测*/
                            display_detect(0);
                            /*显示危险*/
                            display_danger(1);
                            icon_reflash(4);
                            break ;
                        }
                    }

                    Display_Process_Bar(detect_logic.Test_Process, 1);
                }

                break ;

            case DETECT_SAFETY:
                icon_reflash(4);
                detect_logic.Start_Detect = 0 ;
                break ;

            case DETECT_DANGER:
                /*危险闪烁*/
                Refresh_flag = !Refresh_flag ;
                display_danger(Refresh_flag);
                mq2_sensor_interface.led_control(&mq2_sensor_interface, Refresh_flag);
                mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, Refresh_flag);
                break ;

            default:
                break ;
        }
    }
}


FATFS fs;	// FatFs文件系统对象
FIL file;	// 文件对象
FRESULT f_res;     //文件操作结果
uint8_t f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
    f_res = f_mount(fs1, (TCHAR const*)SDPath, 1);
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);//得到磁盘信息及空闲簇数量

    if(res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize; //得到总扇区数
        fre_sect = fre_clust * fs1->csize;        //得到空闲扇区数
        #if _MAX_SS!=512                                  //扇区大小不是512字节,则转换为512字节
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
        #endif
        *total = tot_sect >> 1; //单位为KB
        *free = fre_sect >> 1; //单位为KB
    }

    f_res = f_mount(NULL, (TCHAR const*)SDPath, 1);	// 不再使用，取消挂载
    return res;
}

void sd_show_picture_bin(TCHAR const* OpenOrCreatTXTFileName)//LCD显示SD中 bin 图片 //通过关闭屏幕背光达到卡点效果
{
    UINT br;
    uint16_t i;
    uint8_t data[1200]; //存储五行像素点240 的 16进制颜色信息 240*5=1200
    f_res = f_mount(&fs, (TCHAR const*)SDPath, 1); //挂载时会对SD卡初始化
    f_res = f_open(&file, OpenOrCreatTXTFileName, FA_READ);

    for(i = 0; i < 96; i++)
    {
        f_res = f_read(&file, data, 1200, &br); //x方向240个像素点，每个像素点 2 个
        SPI2_WriteByte(data, 1200); //显示 5 行像素点

    }

    f_close(&file);
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
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_RTC_Init();
    MX_ADC1_Init();
    MX_TIM16_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    PowerOn();
    LCD_Init();
    sd_show_picture_bin("Start_Logo.bin");
    LCD_DisplayOn();
    HAL_Delay(3000);
    Sensor_Register(&mq2_sensor_interface);
    printf("led_status:%d\n", mq2_sensor_interface.get_led_status(&mq2_sensor_interface));
    printf("buzzer_status:%d\n", mq2_sensor_interface.get_buzzer_status(&mq2_sensor_interface));
    /*注册并启动时钟显示定时器  1000ms一次*/
    timer_init(&DataTime_Timer, DataTime_Timer_CallBack, TIMER_TIMEOUT, TIMER_TIMEOUT);
    timer_start(&DataTime_Timer);
    /*按键扫描*/
    timer_init(&Key_Timer, Key_CallBack, 1, 1);
    timer_start(&Key_Timer);
    /*100ms检测一次烟感值*/
    timer_init(&Test_Timer, Test_CallBack, 100, 100);
    timer_start(&Test_Timer);
    LCD_Clear(BLACK);
    Menu_Init();
    //关指示灯
    HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        timer_loop();
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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USART1
                                         | RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_ADC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK | RCC_PLLSAI1_ADC1CLK;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
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
