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
FRESULT f_res;        /* �ļ�������� */

/*Ĭ������*/
#define DEFULT_TASK_SIZE			1500
void default_task(void *pdata);
osThreadDef(default_task, osPriorityLow, 1, DEFULT_TASK_SIZE);

/*��������*/
#define KEY_TASK_SIZE					1500
void StartKeyTask(void  *argument);
osThreadDef(StartKeyTask, osPriorityHigh, 1, KEY_TASK_SIZE);

int $Sub$$main(void)
{
    extern int main(void);
    extern int $Super$$main(void);
    int ret = -1 ;
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
        return -1 ;
    }
    //��ָʾ��
    HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
    //�ص�������main������
    $Super$$main();
    return 0 ;
}

/*��������ϵͳ*/
void start_tencent_os(void)
{
		printf("�ں�������\n");
    /*��ʼ����ѶOS tiny�ں�*/
    osKernelInitialize();
    /*����������һ��Ĭ������*/
    osThreadCreate(osThread(default_task), NULL);
		/*������������*/
    osThreadCreate(osThread(StartKeyTask), NULL);
    /*����TencentOS tiny�ں�*/
    osKernelStart();
}


void gui_test_demo(void)
{
    GUI_Init();
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_CURSOR_Show();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_24B_1);
    GUI_RECT rect ;
    rect.x0 = 0 ;
    rect.y0 = 0 ;
    rect.x1 = 239 ;
    rect.y1 = rect.y0 + 24 ;
    GUI_DispStringInRectWrap("STemWin", &rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_NONE);

    GUI_RECT rect1 ;
    rect1.x0 = 0 ;
    rect1.y0 = 30 ;
    rect1.x1 = 239 ;
    rect1.y1 = rect1.y0 + 24 ;
    GUI_DispStringInRectWrap("Use TencentOS tiny", &rect1, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_NONE);

    GUI_RECT rect2 ;
    rect2.x0 = 0 ;
    rect2.y0 = 60 ;
    rect2.x1 = 239 ;
    rect2.y1 = rect2.y0 + 24 ;
    GUI_DispStringInRectWrap("STemWin Ver:", &rect2, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_NONE);

    GUI_RECT rect3 ;
    rect3.x0 = 0 ;
    rect3.y0 = 90 ;
    rect3.x1 = 239 ;
    rect3.y1 = rect3.y0 + 24 ;
    const char *str = GUI_GetVersionString();
    GUI_DispStringInRectWrap(str, &rect3, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_NONE);
}

/*Ĭ��������*/
void default_task(void *pdata)
{
		gui_test_demo();
		while(1)
		{
			GUI_Delay(100);
		}
}



/*����������*/
void StartKeyTask(void *argument)
{
    __IO uint8_t KeyCode = 255;

    while(1)
    {
        /*��ȡ��ֵ*/
        GetKey(&KeyCode);

        if(255 != KeyCode)
        {
						printf("���¼�������...\n");
            KeyCode = 255 ;
        }

        tos_sleep_ms(5);
    }
}




