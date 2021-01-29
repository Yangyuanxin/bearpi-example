#include "app_init.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_bmp.h"
#include "stm32l4xx_it.h"

FATFS fs;						  /* FatFs�ļ�ϵͳ���� */
FRESULT f_res;                    /* �ļ�������� */
#define START_LOGO	"0:/UI/start_logo/start_logo.bmp"



#define TASK_TIMESLICE     5

//1
/***************������������*************/
#define KEY_TASK_PRIORITY    	 3
#define KEY_TASK_SIZE       	 1500
static rt_thread_t key_task_thread = RT_NULL;
static void Start_Key_Task(void *parameter);
/***************������������*************/

//1
/***************UI��������*************/
#define GUI_TASK_PRIORITY    	 4
#define GUI_TASK_SIZE       	 1500
static rt_thread_t gui_task_thread = RT_NULL;
static void Start_Gui_Task(void *parameter);
/***************UI��������*************/

LCD_Bmp_Show_Para boot_logo_para = {0, 0, START_LOGO};

/*Ӳ��������ʼ��*/
void hardware_init(void)
{
    uint8_t ret;
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_RTC_Init();
    MX_ADC1_Init();
    MX_TIM16_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    __HAL_RCC_CRC_CLK_ENABLE();
		ret = Mount_SD();
    if(ret != 0)
    {
        printf("SD Card mount ERROR\r\n");
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        return ;
    }
}

/*��������ϵͳ*/
void start_rt_thread_os(void)
{
    /*1�����������߳�*/
    key_task_thread = rt_thread_create("key_th",
                                       Start_Key_Task, RT_NULL,
                                       KEY_TASK_SIZE,
                                       KEY_TASK_PRIORITY, TASK_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (key_task_thread != RT_NULL)
        rt_thread_startup(key_task_thread);

    /*2�����������߳�*/
    gui_task_thread = rt_thread_create("gui_th",
                                       Start_Gui_Task, RT_NULL,
                                       GUI_TASK_SIZE,
                                       GUI_TASK_PRIORITY, TASK_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (gui_task_thread != RT_NULL)
        rt_thread_startup(gui_task_thread);
}

/*UI������*/
static void Start_Gui_Task(void *parameter)
{
    GUI_Init();
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_CURSOR_Show();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_24B_1);
    GUI_DispString("Hello World!\r\nUse RT-Thread");
    while(1)
    {
        GUI_Delay(50);
    }
}

/*����������*/
static void Start_Key_Task(void *parameter)
{
		static uint8_t status = 0 ;
    __IO uint8_t KeyCode = 255;
		GUI_COLOR color_array[] = {GUI_RED,GUI_ORANGE,GUI_LIGHTBLUE,GUI_GREEN,GUI_BLUE,GUI_MAGENTA,GUI_GRAY};
    while(1)
    {
        /*��ȡ��ֵ*/
        GetKey(&KeyCode);

        if(255 != KeyCode)
        {
						if(status == 7) 
							status = 0 ;
						GUI_SetBkColor(color_array[status]);
						GUI_Clear();
						GUI_SetColor(GUI_WHITE);
						GUI_SetFont(GUI_FONT_24B_1);
						GUI_DispString("Hello World!\r\nUse RT-Thread");
						status++ ;
            KeyCode = 255 ;
        }

        rt_thread_mdelay(5);
    }
}


