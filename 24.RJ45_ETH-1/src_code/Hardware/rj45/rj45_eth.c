#include "rj45_eth.h"
#include "usart.h"

RJ45HandleTypeDef RJ45r_Handler ;
/*�豸�����ṹ��=======>���ڶ�*/
DevicePortConfigS Deice_Para_Handledef ;
/*�豸�������ýṹ��===>����д*/
DevicePortConfigS Deice_Para_Config_Handledef ;
static void Enable_And_Clear_Data_Packet(void);
static uint8_t RJ45_Check_Cmd_Ack(uint8_t ack);
static void wifi_uart_write_data(uint8_t *in, uint16_t len);
/* ����ch�ַ���sign�����е���� */
int getIndexOfSigns(char ch);
/* ʮ��������ת��Ϊʮ������ */
long hexToDec(char *source);

/****************************************************���������ò���*************************************************************/
/*RJ45����ģʽ*/
uint8_t RJ45_Set_Mode(uint8_t mode, uint16_t delay_ms)
{
    uint8_t Res = 0 ;
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x10 ;
    RJ45r_Handler.RJ45TxBuffer[3] = mode ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 4);

    while(delay_ms--)
    {
        Res = RJ45_Check_Cmd_Ack(0xAA) ;

        if(0 == Res)
            return 0 ;

        Delay_ms(1);
    }

    return ACK_TIMEOUT ;
}

/*����ģ��Ŀ��IP��ַ*/
uint8_t Set_Module_Gobal_Ipaddr(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint16_t delay_ms)
{
    uint8_t Res = 0 ;
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x15 ;
    RJ45r_Handler.RJ45TxBuffer[3] = bit0 ;
    RJ45r_Handler.RJ45TxBuffer[4] = bit1 ;
    RJ45r_Handler.RJ45TxBuffer[5] = bit2 ;
    RJ45r_Handler.RJ45TxBuffer[6] = bit3 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 7);

    while(delay_ms--)
    {
        Res = RJ45_Check_Cmd_Ack(0xAA) ;

        if(0 == Res)
            return 0 ;

        Delay_ms(1);
    }

    return ACK_TIMEOUT ;
}

/*����ģ��Ŀ�Ķ˿ں�*/
uint8_t Set_Module_Gobal_Port_Number(uint16_t number, uint16_t delay_ms)
{
    uint8_t Res = 0 ;
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x16 ;
    RJ45r_Handler.RJ45TxBuffer[3] = number & 0xff ;
    RJ45r_Handler.RJ45TxBuffer[4] = number >> 8 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 5);

    while(delay_ms--)
    {
        Res = RJ45_Check_Cmd_Ack(0xAA) ;

        if(0 == Res)
            return 0 ;

        Delay_ms(1);
    }

    return ACK_TIMEOUT ;
}

/*�������ò�����EEPROM*/
uint8_t Update_Config_Para_To_EEPROM(uint16_t delay_ms)
{
    uint8_t Res = 0 ;
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x0D ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);

    while(delay_ms--)
    {
        Res = RJ45_Check_Cmd_Ack(0xAA) ;

        if(0 == Res)
            return 0 ;

        Delay_ms(1);
    }

    return ACK_TIMEOUT ;
}

/*ִ�����ò���*/
uint8_t Runing_Config_Para_To_EEPROM(uint16_t delay_ms)
{
    uint8_t Res = 0 ;
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x0E ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);

    while(delay_ms--)
    {
        Res = RJ45_Check_Cmd_Ack(0xAA) ;

        if(0 == Res)
            return 0 ;

        Delay_ms(1);
    }

    return ACK_TIMEOUT ;
}

/*����RJ45ģ�����*/
uint8_t Config_RJ45_Module_Para(void)
{
    uint8_t ret = 1;
    Enable_RJ45_Config_Mode();
    Deice_Para_Config_Handledef.bNetMode = 0x01 ;
    ret = RJ45_Set_Mode(Deice_Para_Config_Handledef.bNetMode, 300);

    if(ret != 0)
        return 1;

    Deice_Para_Config_Handledef.gDesIP[0] = 0x78 ;	//120
    Deice_Para_Config_Handledef.gDesIP[1] = 0x4E ;	//78
    Deice_Para_Config_Handledef.gDesIP[2] = 0x7E ;	//126
    Deice_Para_Config_Handledef.gDesIP[3] = 0x86 ;	//134
    ret = Set_Module_Gobal_Ipaddr(Deice_Para_Config_Handledef.gDesIP[0], 	\
                                  Deice_Para_Config_Handledef.gDesIP[1], Deice_Para_Config_Handledef.gDesIP[2], \
                                  Deice_Para_Config_Handledef.gDesIP[3], 300);

    if(ret != 0)
        return 2;

    Deice_Para_Config_Handledef.gNetPort = 9002 ;		//9002
    ret = Set_Module_Gobal_Port_Number(Deice_Para_Config_Handledef.gNetPort, 300);

    if(ret != 0)
        return 3;

    ret = Update_Config_Para_To_EEPROM(300);

    if(ret != 0)
        return 4;

    ret = Runing_Config_Para_To_EEPROM(300);

    if(ret != 0)
        return 5;

    printf("����RJ45ģ��������£�\n");
    printf("1.����RJ45ģ�鹤��ģʽ:%d\n", Deice_Para_Config_Handledef.bNetMode);
    printf("2.����RJ45ģ��Ŀ��IP��ַ:%d.%d.%d.%d\n", Deice_Para_Config_Handledef.gDesIP[0],	\
           Deice_Para_Config_Handledef.gDesIP[1], Deice_Para_Config_Handledef.gDesIP[2],
           Deice_Para_Config_Handledef.gDesIP[3]);
    printf("3.����RJ45ģ��˿ں�:%d\n", Deice_Para_Config_Handledef.gNetPort);
    return 0 ;
}

/*��ȡRJ45ģ�����*/
uint8_t Get_RJ45_Module_Config_Para(void)
{
    printf("��ȡRJ45ģ�����ò������£�\n");
    /*��ȡоƬ����ģʽ*/
    Get_RJ45_Chip_Work_Mode(300);
    printf("1.��ȡоƬ����ģʽ:%d\n", Deice_Para_Handledef.bNetMode);
    /*��ȡоƬĿ��IP��ַ*/
    Get_RJ45_Chip_Gobal_Ipaddr(300);
    printf("2.��ȡĿ��IP��ַ:%d.%d.%d.%d\n", Deice_Para_Handledef.gDesIP[0], Deice_Para_Handledef.gDesIP[1],	\
           Deice_Para_Handledef.gDesIP[2], Deice_Para_Handledef.gDesIP[3]);
    /*��ȡоƬĿ�Ķ˿ں�*/
    Get_RJ45_Chip_Gobal_Port_Number(300);
    printf("3.��ȡоƬĿ�Ķ˿ں�:%d\n", Deice_Para_Handledef.gNetPort);
    /*��ȡоƬMac��ַ*/
    Get_RJ45_Chip_Mac_Addr(300);
    printf("4.��ȡоƬMac��ַ:%d.%d.%d.%d\n", Deice_Para_Handledef.bMacAddr[0], Deice_Para_Handledef.bMacAddr[1],	\
           Deice_Para_Handledef.bMacAddr[2], Deice_Para_Handledef.bMacAddr[3]);
    return 0 ;
}
/****************************************************���������ò���*************************************************************/

/****************************************************�����ǻ�ȡ����*************************************************************/
/*��ȡоƬ����ģʽ*/
void Get_RJ45_Chip_Work_Mode(uint16_t delay_ms)
{
    Enable_RJ45_Config_Mode();
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x60 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);
    Delay_ms(delay_ms);
    Deice_Para_Handledef.bNetMode = RJ45r_Handler.RJ45RxBuffer[0];
}

/*��ȡоƬĿ��IP��ַ*/
void Get_RJ45_Chip_Gobal_Ipaddr(uint16_t delay_ms)
{
    Enable_RJ45_Config_Mode();
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x65 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);
    Delay_ms(delay_ms);
    Deice_Para_Handledef.gDesIP[0] = RJ45r_Handler.RJ45RxBuffer[0] ;
    Deice_Para_Handledef.gDesIP[1] = RJ45r_Handler.RJ45RxBuffer[1] ;
    Deice_Para_Handledef.gDesIP[2] = RJ45r_Handler.RJ45RxBuffer[2] ;
    Deice_Para_Handledef.gDesIP[3] = RJ45r_Handler.RJ45RxBuffer[3] ;
}

/*��ȡоƬĿ�Ķ˿ں�*/
void Get_RJ45_Chip_Gobal_Port_Number(uint16_t delay_ms)
{
    Enable_RJ45_Config_Mode();
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x66 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);
    Delay_ms(delay_ms);
    Deice_Para_Handledef.gNetPort = RJ45r_Handler.RJ45RxBuffer[1] << 8 | RJ45r_Handler.RJ45RxBuffer[0];
}

/*��ȡоƬMac��ַ*/
void Get_RJ45_Chip_Mac_Addr(uint16_t delay_ms)
{
    Enable_RJ45_Config_Mode();
    Enable_And_Clear_Data_Packet();
    RJ45r_Handler.RJ45TxBuffer[0] = 0x57 ;
    RJ45r_Handler.RJ45TxBuffer[1] = 0xab ;
    RJ45r_Handler.RJ45TxBuffer[2] = 0x81 ;
    wifi_uart_write_data( RJ45r_Handler.RJ45TxBuffer, 3);
    Delay_ms(delay_ms);
    Deice_Para_Handledef.bMacAddr[0] = RJ45r_Handler.RJ45RxBuffer[0] ;
    Deice_Para_Handledef.bMacAddr[1] = RJ45r_Handler.RJ45RxBuffer[1] ;
    Deice_Para_Handledef.bMacAddr[2] = RJ45r_Handler.RJ45RxBuffer[2] ;
    Deice_Para_Handledef.bMacAddr[3] = RJ45r_Handler.RJ45RxBuffer[3] ;
}
/*******************************************����Ϊ�ṩ������ͺʹ���ĺ���*********************************************/

/*ʹ��RJ45����ģʽ*/
void Enable_RJ45_Config_Mode(void)
{
    /*�رտ����жϣ���ʱ�����շ�����ģʽ�����ݣ�ֻ����ģ�鱾��ָ���շ��Ļظ�����*/
    __HAL_UART_DISABLE_IT(UART_PORT, UART_IT_IDLE);
    HAL_GPIO_WritePin(RJ45_CONFIG_PORT, RJ45_CONFIG_PIN, GPIO_PIN_RESET);
}

/*ʧ��RJ45����ģʽ*/
void Disable_RJ45_Config_Mode(void)
{
    HAL_GPIO_WritePin(RJ45_CONFIG_PORT, RJ45_CONFIG_PIN, GPIO_PIN_SET);
}

/*���TCP״̬������0��Ϊδ���ӣ�����1��������*/
uint8_t Check_TCP_Status(void)
{
    uint8_t status ;
    status = HAL_GPIO_ReadPin(RJ45_READ_TCP_STATUS_PORT, RJ45_READ_TCP_STATUS_PIN);

    if(0 == status)
        return 1 ;

    return 0 ;
}

/*��������͸��ģʽ*/
uint8_t Enter_Data_Penetrate_Mode(void)
{
    /*ʧ������ģʽ*/
    Disable_RJ45_Config_Mode();
    /*ʹ��DMA��������ݰ�*/
    Enable_And_Clear_Data_Packet();
    /*���������жϣ���ʱ���յ���TCP/IPЭ���շ�������*/
    __HAL_UART_ENABLE_IT(UART_PORT, UART_IT_IDLE);
    Deice_Para_Config_Handledef.dataMode = 1 ;
    return 0 ;
}

/*�˳�����͸��ģʽ*/
uint8_t Quit_Data_Penetrate_Mode(void)
{
    /*ʧ������ģʽ*/
    Enable_RJ45_Config_Mode();
    /*ʹ��DMA��������ݰ�*/
    Enable_And_Clear_Data_Packet();
    return 0 ;
}


/*ʹ��DMA��������ݰ�*/
static void Enable_And_Clear_Data_Packet(void)
{
    HAL_UART_DMAStop(UART_PORT);
    memset(RJ45r_Handler.RJ45TxBuffer, 0, RJ45_TXBUFFER_SIZE);
    memset(RJ45r_Handler.RJ45RxBuffer, 0, RJ45_RXBUFFER_SIZE);
    HAL_UART_Receive_DMA(UART_PORT, RJ45r_Handler.RJ45RxBuffer, RJ45_RXBUFFER_SIZE);
}

/*0 �ɹ�  ����ʧ��*/
static uint8_t RJ45_Check_Cmd_Ack(uint8_t ack)
{
    if(RJ45r_Handler.RJ45RxBuffer[0] == ack)
        return 0;

    return 1;
}

/*SDK���ڷ���һ���ֽ�*/
void Uart_PutChar(uint8_t value)
{
    HAL_UART_Transmit(UART_PORT, (uint8_t *)&value, 1, 1000);
}

/**
 * @brief  ���ڷ�������
 * @param[in] {value} ����Ҫ���͵�1�ֽ�����
 * @return Null
 */
void uart_transmit_output(uint8_t value)
{
    Uart_PutChar(value);	                                //���ڷ��ͺ���
}

/**
 * @brief  ���ڷ���һ������
 * @param[in] {in} ���ͻ���ָ��
 * @param[in] {len} ���ݷ��ͳ���
 * @return Null
 */
static void wifi_uart_write_data(uint8_t *in, uint16_t len)
{
    if((NULL == in) || (0 == len))
    {
        return;
    }

    while(len --)
    {
        uart_transmit_output(*in);
        in ++;
    }
}

#ifdef CMSIS_RTOS_SUPPORT
void rtos_sched_lock(void)
{

}

void rtos_sched_unlock(void)
{

}
#endif
//RJ45��������͸�����ݺ�����������͸��ģʽ��ʹ��
void RJ45_Send_NetWork_Penetrate_Data(char* fmt, ...)
{
    if(1 == Deice_Para_Config_Handledef.dataMode)
    {
        /*����������*/
        #ifdef CMSIS_RTOS_SUPPORT
        rtos_sched_lock();
        #endif
        uint16_t len;
        va_list ap;
        va_start(ap, fmt);
        vsprintf((char*)RJ45r_Handler.RJ45TxBuffer, fmt, ap);
        va_end(ap);
        len = strlen((const char*)RJ45r_Handler.RJ45TxBuffer);
        HAL_UART_Transmit(UART_PORT, RJ45r_Handler.RJ45TxBuffer, len, 1000);
        /*����������*/
        #ifdef CMSIS_RTOS_SUPPORT
        rtos_sched_unlock();
        #endif
    }
}

/* ����ch�ַ���sign�����е���� */
int getIndexOfSigns(char ch)
{
    if(ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }

    if(ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }

    if(ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }

    return -1;
}

/* ʮ��������ת��Ϊʮ������ */
long hexToDec(char *source)
{
    long sum = 0;
    long t = 1;
    int i, len;

    len = strlen(source);

    for(i = len - 1; i >= 0; i--)
    {
        sum += t * getIndexOfSigns(*(source + i));
        t *= 16;
    }

    return sum;
}
/*******************************************����Ϊ�ṩ������ͺʹ���ĺ���*********************************************/
