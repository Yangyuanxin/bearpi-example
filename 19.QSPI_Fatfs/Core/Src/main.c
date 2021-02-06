/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "fatfs.h"
#include "i2c.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_w25q64.h"
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
/*挂载文件系统*/
uint8_t work[_MAX_SS] ;
void Mount_Fatfs(void)
{
    retUSER = f_mount(&USERFatFS, USERPath, 1);

    if(retUSER != FR_OK)
    {
        if(retUSER == FR_NO_FILESYSTEM)
        {
            printf("f_mount 没有文件系统,开始格式化spi-flash\r\n");
            retUSER = f_mkfs(USERPath, FM_ANY, 0, work, sizeof(work));

            if(retUSER != FR_OK)
            {
                printf("f_mkfs 格式化失败，err = %d\r\n", retUSER);
            }
            else
            {
                printf("格式化成功，开始重新挂载spi-flash\r\n");
                retUSER = f_mount(&USERFatFS, USERPath, 1);

                if(retUSER != FR_OK)
                {
                    printf("f_mount 发生错误，err = %d\r\n", retUSER);
                }
                else
                {
                    printf("spi-flash文件系统挂载成功\r\n");
                }
            }
        }
        else
        {
            printf("f_mount 发生其他错误，err = %d\r\n", retUSER);
        }
    }
    else
        printf("spi-flash文件系统挂载成功\r\n");
}

/*获取盘内存*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
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
    return res;
}

unsigned int count = 0;
unsigned char work[4096] = {0};
unsigned char read_buf[50] = {0};
unsigned char write_buf[50] = "hello BearPi\r\n";
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
		uint8_t res ;
		uint32_t Total = 0; //读取FLASH总容量
    uint32_t Free = 0; //读取FLASH剩余容量
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
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_QUADSPI_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    Mount_Fatfs();
    f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //获取SD卡总容量和剩余容量
		printf("当前Fatfs总容量:%dKB==>%dMB 剩余容量:%dKB==>%dMB\n",Total,Total/1024,Free,Free/1024);
		
		/*----------------------- 文件系统测试：写测试 -----------------------------*/
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    res = f_open(&USERFile, "0:BearPi.txt", FA_OPEN_ALWAYS | FA_WRITE);
 
    if(res == FR_OK)
    {
        printf("打开/创建BearPi.txt文件成功，向文件写入数据。\r\n");
        res = f_write(&USERFile, write_buf, strlen((const char *)write_buf), &count);
 
        if(res != FR_OK)
        {
            printf("f_write 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi.txt文件\r\n");
            count = 0;
            f_close(&USERFile);
        }
        else
        {
            printf("文件写入成功，写入字节数据：%d\n", count);
            printf("向文件写入的数据为：\r\n%s\r\n", write_buf);
            printf("关闭打开的BearPi.txt文件\r\n");
            count = 0;
            f_close(&USERFile);
        }
    }
    else printf("打开/创建BearPi.txt文件失败，err = %d\r\n", res);
 
    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res = f_open(&USERFile, "0:BearPi.txt", FA_OPEN_EXISTING | FA_READ);
 
    if(res == FR_OK)
    {
        printf("打开BearPi.txt文件成功\r\n");
        res = f_read(&USERFile, read_buf, sizeof(read_buf), &count);
 
        if(res != FR_OK)
        {
            printf("f_read 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi.txt文件\r\n");
            f_close(&USERFile);
        }
        else
        {
            printf("文件读取成功，读取字节数据：%d\n", count);
            printf("向文件读取的数据为：\r\n%s\r\n", read_buf);
            printf("关闭打开的BearPi.txt文件\r\n");
            f_close(&USERFile);
        }
    }
    else printf("打开BearPi.txt文件失败，err = %d\r\n", res);
 
 
 
    /*------------------- 不再使用文件系统，取消挂载文件系统 ------------------------------------*/
    printf("不再使用文件系统，取消挂载文件系统\r\n");
    res = f_mount(NULL, "0:", 1);
 
    if(res == FR_OK) printf("取消挂载文件系统成功\r\n");
    else    printf("取消挂载文件系统失败，err = %d\r\n", res);
 
    printf("文件系统测试结束\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
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
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
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

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;

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
