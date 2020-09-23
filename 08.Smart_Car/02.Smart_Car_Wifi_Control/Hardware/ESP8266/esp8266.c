#include "esp8266.h"
#include "usart.h"
#include <stdarg.h>
#include "lcd.h"

wifi_ap_info esp8266_info ;
static uint8_t AT_Check_Answer(uint8_t *ack);
static uint8_t AT_Cmd_Answer(uint8_t* success, uint8_t* fail, uint32_t timeout);

#define delay_ms(s) HAL_Delay(s)

//��λ
uint8_t WIFI_RST(void)
{
    uint8_t res = 0xff;
    wifi_init_printf("AT+RST\r\n");
    res = AT_Cmd_Answer((uint8_t*)"OK", NULL, 5000);
    delay_ms(1000);
    return res;
}

//����
uint8_t WIFI_Test(void)
{
    uint8_t res = 0xff;
    wifi_init_printf("AT\r\n");
    res = AT_Cmd_Answer((uint8_t*)"OK\r\n", NULL, 200);
    return  res;
}

//����ESP8266�Ƿ����,����true,��ʾ���óɹ�
uint8_t WIFI_SET_ATE(uint8_t status)
{
    uint8_t res = 1 ;
    wifi_init_printf("ATE%d\r\n", status);
    res = AT_Cmd_Answer((uint8_t*)"OK\r\n", (uint8_t*)"ERROR\r\n", 200);

    if(0 != res)
        return 1 ;

    return 0 ;
}

//STAģʽ
uint8_t WIFI_STA_MODE(void)
{
    uint8_t res = 0xff;

    wifi_init_printf("AT+CWMODE=1\r\n");
    res = AT_Cmd_Answer((uint8_t*)"OK", (uint8_t*)"ERROR", 500);
    return res;
}

//����·��
uint8_t WIFI_CWJAP(uint8_t* ssid, uint8_t* password)
{
    uint8_t res = 0xff;
    wifi_init_printf("AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    res = AT_Cmd_Answer((uint8_t*)"OK", NULL, 2000);
    return res;
}

//Э�顢 ������IP���˿�
uint8_t WIFI_CIPSTART(uint8_t* type, uint8_t* addr, uint32_t port)
{
    uint8_t res = 0xff;

    wifi_init_printf("AT+CIPSTART=\"%s\",\"%s\",%d\r\n", type, addr, port);
    res = AT_Cmd_Answer((uint8_t*)"OK", (uint8_t*)"ERROR", 2000);
    return res;
}

//���ÿ���͸��ģʽ
uint8_t WIFI_CIPMODE(void)
{
    uint8_t res = 0xff;

    wifi_init_printf("AT+CIPMODE=1\r\n");
    res = AT_Cmd_Answer((uint8_t*)"OK", (uint8_t*)"ERROR", 200);
    return res;
}

//��ʼ͸��
uint8_t WIFI_CIPSEND(void)
{
    uint8_t res = 0xff;

    wifi_init_printf("AT+CIPSEND\r\n");
    res = AT_Cmd_Answer((uint8_t*)"OK", (uint8_t*)"ERROR", 300);
    return res;
}

extern __IO int wifi_connect_flag ;
//WIFI��ʼ��
void WIFI_Init(uint8_t *ssid, uint8_t *password)
{
    int ret = 0 ;
    uint8_t err_code ;
    static int wifi_status_process = 0;
    int32_t error_count = 3;
WIFI_INIT:
    WIFI_RST();

    //�ж�wifi�ڲ�����
    while(error_count--)
    {
        if(WIFI_Test() == 0)
            break;
    }

    printf("wifi����\r\n");
    error_count = 3 ;
    WIFI_SET_ATE(0);
WIFI_PROCESS_CONNECT:

    while(1)
    {
        //wifi����״̬��
        switch(wifi_status_process)
        {
            //����ģʽ
            case 0:
                if(0 != WIFI_STA_MODE())
                {
                    printf("����ģʽ����״̬\r\n");
                    goto WIFI_INIT ;
                }

                printf("1.wifi����STAģʽ�ɹ�!\r\n");
                wifi_status_process = 1 ;
                break ;

            //����·����
            case 1:
                if(0 != WIFI_CWJAP(ssid, password))
                {
                    printf("����·�������� ssid:%s password:%s\r\n", ssid, password);
                    wifi_status_process = 1 ;
                    break ;
                }

                wifi_status_process = 2 ;
                printf("2.wifi����·����:%s�ɹ�\r\n", ssid);
                break ;

            //���ӷ�����
            case 2:
                if(0 != WIFI_CIPSTART((uint8_t*)"TCP", (uint8_t*)"192.168.4.1", 8080))
                {
                    printf("���ӷ���������\r\n");
                    wifi_status_process = 1 ;
                    break ;
                }

                printf("3.wifi���ӷ�����:%s   �˿ں�:%d�ɹ�\r\n", (uint8_t*)"192.168.4.1", 8080);
                wifi_status_process = 3 ;
                break ;
        }

        //���Ϊok��������ѭ��
        if(3 == wifi_status_process)
            break ;
    }

    err_code = WIFI_CIPMODE();

    if(err_code != 0)
        return  ;

    printf("����͸��ģʽ�ɹ�\n");
    err_code = WIFI_CIPSEND();

    if(err_code != 0)
        return  ;

    printf("����͸��ģʽ�ɹ�\n");
    wifi_connect_flag = 1 ;
    LCD_ShowString(0, 100, 240, 32, 32, (char *)ssid);//��ʾ�ַ����������С32*32
    LCD_ShowString(0, 132, 240, 32, 32, (char *)password);//��ʾ�ַ����������С32*32
    LCD_ShowString(0, 164, 240, 32, 32, (char *)"192.168.4.1");//��ʾ�ַ����������С32*32
    LCD_ShowString(0, 196, 240, 32, 32, (char *)"8080");//��ʾ�ַ����������С32*32
}




void wifi_init_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;

    HAL_UART_DMAStop(&hlpuart1);
    HAL_Delay(100);
    memset((char *)esp8266_info.tx_buffer, 0, TX_BUFF_SIZE);
    memset((char *)esp8266_info.rx_buffer, 0, RX_BUFF_SIZE);
    HAL_UART_Receive_DMA(&hlpuart1, esp8266_info.rx_buffer, RX_BUFF_SIZE);

    va_start(args, format);
    length = vsnprintf((char *)esp8266_info.tx_buffer, sizeof(esp8266_info.tx_buffer), (char *)format, args);
    va_end(args);
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)esp8266_info.tx_buffer, length, HAL_MAX_DELAY);
}

/*0 �ɹ�  ����ʧ��*/
static uint8_t AT_Check_Answer(uint8_t *ack)
{
    if(strstr((char *)esp8266_info.rx_buffer, (const char *)ack) != NULL)
        return 0;

    return 1;
}

//0 �ɹ�  ����ʧ�ܡ�����Ӧ��Ϊ�յ�ʱ���ֱ�ӷ��ء�
static uint8_t AT_Cmd_Answer(uint8_t* success, uint8_t* fail, uint32_t timeout)
{
    uint32_t rxlen = 0, i;

    if(success == NULL && fail == NULL)
        return 0;

    while(timeout--)
    {
        if(success)
        {
            if(AT_Check_Answer(success) == 0)
            {
                return 0;
            }
        }

        if(fail)
        {
            if(AT_Check_Answer(fail) == 0)
                return 1;
        }

        if(AT_Check_Answer(success) == 0)
            return 0;

        if(AT_Check_Answer(fail) == 0)
            return 1;

        HAL_Delay(1);
    }

    return 2;
}

