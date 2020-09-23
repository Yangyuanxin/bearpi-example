#include "esp8266.h"
#include "usart.h"
#include <stdarg.h>
#include "tim.h"

wifi_ap_info esp8266_info ;
void Reset_Wifi_Para(void);
static void wifi_timeout_callback(void);
static uint8_t AT_Check_Answer(char *ack);
static void Wifi_Recv_Cmd_Process(void);


/*����*/
void WIFI_Test(void)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    /*��ǰΪ����״̬*/
    if(1 == esp8266_info.tx_flag)
    {
        /*��λ����*/
        Reset_Wifi_Para();
        /*���Ͳ���ָ��*/
        wifi_send_cmd(WIFI_AT_TEST);
        /*������״̬����Ϊ0����Ϊ����״̬*/
        esp8266_info.tx_flag = 0 ;
        /*��ն�ʱ������*/
        esp8266_info.wifi_timer_count = 0 ;
        /*������ʱ������־*/
        esp8266_info.wifi_completed_flag = 0 ;
    }
    /*��ǰΪ����״̬*/
    else
    {
        /*�ж϶�ʱ������WIFI_TEST_TIMEOUT==>1000ms��û�У�*/
        if(WIFI_TEST_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            /*�رն�ʱ������־*/
            esp8266_info.wifi_completed_flag = 1 ;
            /*��ն�ʱ������*/
            esp8266_info.wifi_timer_count = 0 ;
            /*���DMA���ջ������Ƿ����OK�Ӵ�*/
            ret = AT_Check_Answer("OK");

            /*ʧ�ܣ����󳬹�3�Σ����س���״̬*/
            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI��ʼ��ʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                /*��״̬���Ϊ��һ��ָ��Ĵ�������*/
                esp8266_info.wifi_apr_status = ITEM_WIFI_SATE ;
                printf("wifi���Գɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}

/*����ESP8266�Ƿ����*/
void WIFI_SET_ATE(uint8_t status)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_ATE_SET, status);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_SET_ATE_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI���û���ʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_SMODE ;
                printf("wifi���û��Գɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}


/*����wifiģʽ*/
void WIFI_SET_MODE(uint8_t mode)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_AT_SET_MODE, mode);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_SET_MODE_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI����APģʽʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_BUIAP ;
                printf("wifi����APģʽ�ɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}

/*����wifi�ȵ�*/
void WIFI_BUILD_AP_INFO(const char *ssid, const char *password, int chl, int ecn)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_AT_SAP, ssid, password, chl, ecn);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_BUILD_AP_INFO_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI�����ȵ�ʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_CMULT ;
                printf("wifi�����ȵ�%s�ɹ�!  �ظ�%s\n", ssid, esp8266_info.rx_buffer);
            }
        }
    }
}


/*���ö�����ģʽ*/
void WIFI_CONFIG_MULTPLE_CONNECT(uint8_t status)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_AT_MULTPLE, status);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_CONFIG_MULTPLE_CONNECT_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI����������ʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_OSERV ;
                printf("wifi���ö�����ģʽ�ɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}

/*������������������Ͽ�ʱ��*/
void WIFI_CONFIG_SERVER_TIMEOUT(int TimeOut)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        ++err_count ;
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_SET_STO, TimeOut);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_CONFIG_MULTPLE_CONNECT_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI���ó�ʱʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_VADDR ;
                printf("wifi���ó�ʱ�ɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}


/*����������ģʽ*/
void WIFI_OPEN_SERVER_MODE(uint8_t mode, int Port)
{
    uint8_t ret = 0 ;
    static uint8_t err_count = 0 ;

    if(1 == esp8266_info.tx_flag)
    {
        ++err_count ;
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_OPEN_SMODE, mode, Port);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_OPEN_SERVER_MODE_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI����������ģʽʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                esp8266_info.wifi_apr_status = ITEM_WIFI_STIMO ;
                printf("wifi����������ģʽ�ɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
            }
        }
    }
}

/*�鿴��ַ*/
void WIFI_VIEW_IPADDR(void)
{
    uint8_t ret = 0 ;
    char ip_buf[20] = {0};
    char display_buf[20] = {0};
    static uint8_t err_count = 0 ;
    uint8_t *temp1 = NULL, *temp2 = NULL;

    if(1 == esp8266_info.tx_flag)
    {
        ++err_count ;
        Reset_Wifi_Para();
        wifi_send_cmd(WIFI_VIEW_ADDR);
        esp8266_info.tx_flag = 0 ;
        esp8266_info.wifi_timer_count = 0 ;
        esp8266_info.wifi_completed_flag = 0 ;
    }
    else
    {
        if(WIFI_VIEW_IPADDR_TIMEOUT == esp8266_info.wifi_timer_count)
        {
            esp8266_info.wifi_completed_flag = 1 ;
            esp8266_info.wifi_timer_count = 0 ;
            ret = AT_Check_Answer("OK");

            if(ret != 0)
            {
                esp8266_info.tx_flag = 1 ;
                ++err_count;

                if(err_count > 3)
                {
                    err_count = 0 ;
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;
                    printf("WIFI��ȡAPIP��ַʧ��\n");
                }
            }
            else
            {
                esp8266_info.tx_flag = 1 ;
                printf("wifi��ѯAPIP��ַ�ɹ�!  �ظ�%s\n", esp8266_info.rx_buffer);
                temp1 = (uint8_t*)strstr((const char*)esp8266_info.rx_buffer, "\"");
                temp2 = (uint8_t*)strstr((const char*)(temp1 + 1), "\"");

                if(temp1 == NULL || temp2 == NULL)
                    esp8266_info.wifi_apr_status = ITEM_WIFI_ERROR ;

                *temp2 = 0;
                sprintf((char *)ip_buf, "%s", temp1 + 1);
                sprintf(display_buf, "APIP:%s", ip_buf);
                printf("%s\n", display_buf);
                memset(ip_buf, 0, 20);
                sprintf(display_buf, "PORT:%d", AP_PORT);
                printf("%s\n", display_buf);
                HAL_UART_DMAStop(&huart2);
                memset(esp8266_info.rx_buffer, 0, RX_BUFF_SIZE);
                HAL_UART_Receive_DMA(&huart2, esp8266_info.rx_buffer, RX_BUFF_SIZE);
                esp8266_info.wifi_apr_status = ITEM_WIFI_GDATA ;
            }
        }
    }
}


/*ESP8266��ΪAPģʽ��������*/
void ESP8266_AP_Mode_Setting(void)
{
    static uint8_t error_flag = 0 ;

    switch(esp8266_info.wifi_apr_status)
    {
        case ITEM_WIFI_TEST:
            WIFI_Test();
            break ;

        case ITEM_WIFI_SATE:
            WIFI_SET_ATE(1);
            break ;

        case ITEM_WIFI_SMODE:
            WIFI_SET_MODE(2);
            break ;

        case ITEM_WIFI_BUIAP:
            WIFI_BUILD_AP_INFO(WIFI_HOT_SPOT_SSID, WIFI_HOT_SPOT_PASSWORD, 1, 4);
            break ;

        case ITEM_WIFI_CMULT:
            WIFI_CONFIG_MULTPLE_CONNECT(1);
            break ;

        case ITEM_WIFI_OSERV:
            WIFI_OPEN_SERVER_MODE(1, AP_PORT);
            break ;

        case ITEM_WIFI_STIMO:
            WIFI_CONFIG_SERVER_TIMEOUT(0);
            break ;

        case ITEM_WIFI_VADDR:
            WIFI_VIEW_IPADDR();
            break ;

        case ITEM_WIFI_GDATA:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
            Wifi_Recv_Cmd_Process();
            break ;

        default:
            if(0 == error_flag)
            {
                error_flag = 1 ;

                if(ITEM_WIFI_ERROR == esp8266_info.wifi_apr_status)
                {
                    printf("WIFI����\n");
                }
            }

            break ;
    }
}


#define CCR0_Val 0
#define CCR1_Val 125
#define CCR2_Val 250
#define CCR3_Val 500
#define CCR4_Val 750
#define CCR5_Val 999

static void Motor_PWM_SetValue(TIM_HandleTypeDef *htim, uint32_t Channe, unsigned short value)
{
    TIM_OC_InitTypeDef sConfigOC;

    if(value >= 1000 - 1) value = 999;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, Channe) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_TIM_PWM_Start(htim, Channe) != HAL_OK)
    {
        Error_Handler();
    }
}

void Moto_Stop(void)
{
    Motor_PWM_SetValue(&htim2, TIM_CHANNEL_1, CCR0_Val);
    Motor_PWM_SetValue(&htim2, TIM_CHANNEL_2, CCR0_Val);
    Motor_PWM_SetValue(&htim2, TIM_CHANNEL_3, CCR0_Val);
    Motor_PWM_SetValue(&htim2, TIM_CHANNEL_4, CCR0_Val);

    Motor_PWM_SetValue(&htim1, TIM_CHANNEL_1, CCR0_Val);
    Motor_PWM_SetValue(&htim1, TIM_CHANNEL_2, CCR0_Val);
    Motor_PWM_SetValue(&htim1, TIM_CHANNEL_3, CCR0_Val);
    Motor_PWM_SetValue(&htim1, TIM_CHANNEL_4, CCR0_Val);
}


/*wifi����ָ���*/
static void Wifi_Recv_Cmd_Process(void)
{
    static int cmd_index = 0 ;
    char *cmd[] = {"LEFT", "RIGHT", "GO", "BACK", "STOP"};

    if(strstr((char *)esp8266_info.rx_buffer, cmd[cmd_index]) != NULL)
    {
        HAL_UART_DMAStop(&huart2);
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        printf("���յ��ͻ��˷�����ָ��:%s\n", esp8266_info.rx_buffer);

        switch(cmd_index)
        {
            //��ת
            case 0:
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_1, CCR0_Val); //0		==> ������ǰ�ֺ�
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_2, CCR2_Val); //1		==> ������ǰ��ǰ	===> ��
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_3, CCR5_Val); //1		==> ������ǰ��ǰ	===> ��
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_4, CCR0_Val); //0		==> ������ǰ�ֺ�

                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_1, CCR2_Val); //1		==> ���������ǰ	===> ��
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_2, CCR0_Val); //0		==> ��������ֺ�
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_3, CCR0_Val); //0		==> �����Һ��ֺ�
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_4, CCR5_Val); //1		==> �����Һ���ǰ  ===> ��

                break ;

            //��ת
            case 1:
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_1, CCR0_Val); //0
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_2, CCR5_Val); //1 ��
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_3, CCR2_Val); //1 ��
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_4, CCR0_Val); //0

                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_1, CCR5_Val); //1 ��
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_2, CCR0_Val); //0
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_3, CCR0_Val); //0
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_4, CCR2_Val); //1 ��
                break ;

            //ǰ��
            case 2:
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_1, CCR0_Val); //0
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_2, CCR5_Val); //1
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_3, CCR5_Val); //1
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_4, CCR0_Val); //0

                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_1, CCR5_Val); //1
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_2, CCR0_Val); //0
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_3, CCR0_Val); //0
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_4, CCR5_Val); //1
                break ;

            //����
            case 3:
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_1, CCR5_Val); //1
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_2, CCR0_Val); //0
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_3, CCR0_Val); //0
                Motor_PWM_SetValue(&htim1, TIM_CHANNEL_4, CCR5_Val); //1

                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_1, CCR0_Val); //0
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_2, CCR5_Val); //1
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_3, CCR5_Val); //1
                Motor_PWM_SetValue(&htim2, TIM_CHANNEL_4, CCR0_Val); //0
                break ;

            //ֹͣ
            case 4:

                Moto_Stop();
                break ;
        }

        memset(esp8266_info.rx_buffer, 0, RX_BUFF_SIZE);
        HAL_UART_Receive_DMA(&huart2, esp8266_info.rx_buffer, RX_BUFF_SIZE);
    }

    ++cmd_index ;

    if(5 == cmd_index)
        cmd_index = 0 ;
}




/*ESP8266��ΪAPģʽ���г�ʼ��*/
void Init_ESP8266_AP_Mode(void)
{
    esp8266_info.tx_flag = 1 ;
    esp8266_info.wifi_apr_status = ITEM_WIFI_TEST ;
    esp8266_info.wifi_timer_count = 0 ;
    esp8266_info.wifi_completed_flag = 1 ;
    esp8266_info.wifi_timeout_cb =  wifi_timeout_callback ;
    /*����1ms�����ʱ��*/
    timer_init(&esp8266_info.wifi_timer, esp8266_info.wifi_timeout_cb, 1, 1);
    timer_start(&esp8266_info.wifi_timer);
}

/*��λwifi�շ�����*/
void Reset_Wifi_Para(void)
{
    HAL_UART_DMAStop(&huart2);
    memset(esp8266_info.tx_buffer, 0, TX_BUFF_SIZE);
    memset(esp8266_info.rx_buffer, 0, RX_BUFF_SIZE);
    HAL_UART_Receive_DMA(&huart2, esp8266_info.rx_buffer, RX_BUFF_SIZE);
}

/*wifi��������*/
void wifi_send_cmd(const char *format, ...)
{
    va_list args;
    uint32_t length;
    va_start(args, format);
    length = vsnprintf((char *)esp8266_info.tx_buffer, sizeof(esp8266_info.tx_buffer), (char *)format, args);
    va_end(args);
    HAL_UART_Transmit(&huart2, (uint8_t *)esp8266_info.tx_buffer, length, HAL_MAX_DELAY);
}

/*0 �ɹ�  ����ʧ��*/
static uint8_t AT_Check_Answer(char *ack)
{
    if(strstr((char *)esp8266_info.rx_buffer, ack) != NULL)
        return 0;

    return 1;
}

static void wifi_timeout_callback(void)
{
    if(0 == esp8266_info.wifi_completed_flag)
        ++esp8266_info.wifi_timer_count ;
}
