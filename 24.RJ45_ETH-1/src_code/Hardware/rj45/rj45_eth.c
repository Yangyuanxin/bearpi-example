#include "rj45_eth.h"
#include "usart.h"

RJ45HandleTypeDef RJ45r_Handler ;
/*设备参数结构体=======>用于读*/
DevicePortConfigS Deice_Para_Handledef ;
/*设备参数配置结构体===>用于写*/
DevicePortConfigS Deice_Para_Config_Handledef ;
static void Enable_And_Clear_Data_Packet(void);
static uint8_t RJ45_Check_Cmd_Ack(uint8_t ack);
static void wifi_uart_write_data(uint8_t *in, uint16_t len);
/* 返回ch字符在sign数组中的序号 */
int getIndexOfSigns(char ch);
/* 十六进制数转换为十进制数 */
long hexToDec(char *source);

/****************************************************以下是配置参数*************************************************************/
/*RJ45设置模式*/
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

/*设置模块目的IP地址*/
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

/*设置模块目的端口号*/
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

/*更新配置参数到EEPROM*/
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

/*执行配置参数*/
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

/*配置RJ45模块参数*/
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

    printf("配置RJ45模块参数如下：\n");
    printf("1.配置RJ45模块工作模式:%d\n", Deice_Para_Config_Handledef.bNetMode);
    printf("2.配置RJ45模块目的IP地址:%d.%d.%d.%d\n", Deice_Para_Config_Handledef.gDesIP[0],	\
           Deice_Para_Config_Handledef.gDesIP[1], Deice_Para_Config_Handledef.gDesIP[2],
           Deice_Para_Config_Handledef.gDesIP[3]);
    printf("3.配置RJ45模块端口号:%d\n", Deice_Para_Config_Handledef.gNetPort);
    return 0 ;
}

/*获取RJ45模块参数*/
uint8_t Get_RJ45_Module_Config_Para(void)
{
    printf("读取RJ45模块配置参数如下：\n");
    /*读取芯片工作模式*/
    Get_RJ45_Chip_Work_Mode(300);
    printf("1.读取芯片工作模式:%d\n", Deice_Para_Handledef.bNetMode);
    /*读取芯片目的IP地址*/
    Get_RJ45_Chip_Gobal_Ipaddr(300);
    printf("2.读取目的IP地址:%d.%d.%d.%d\n", Deice_Para_Handledef.gDesIP[0], Deice_Para_Handledef.gDesIP[1],	\
           Deice_Para_Handledef.gDesIP[2], Deice_Para_Handledef.gDesIP[3]);
    /*读取芯片目的端口号*/
    Get_RJ45_Chip_Gobal_Port_Number(300);
    printf("3.读取芯片目的端口号:%d\n", Deice_Para_Handledef.gNetPort);
    /*读取芯片Mac地址*/
    Get_RJ45_Chip_Mac_Addr(300);
    printf("4.读取芯片Mac地址:%d.%d.%d.%d\n", Deice_Para_Handledef.bMacAddr[0], Deice_Para_Handledef.bMacAddr[1],	\
           Deice_Para_Handledef.bMacAddr[2], Deice_Para_Handledef.bMacAddr[3]);
    return 0 ;
}
/****************************************************以上是配置参数*************************************************************/

/****************************************************以下是获取参数*************************************************************/
/*获取芯片工作模式*/
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

/*获取芯片目的IP地址*/
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

/*获取芯片目的端口号*/
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

/*获取芯片Mac地址*/
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
/*******************************************以下为提供给命令发送和处理的函数*********************************************/

/*使能RJ45配置模式*/
void Enable_RJ45_Config_Mode(void)
{
    /*关闭空闲中断，此时不接收非配置模式的数据，只接收模块本身指令收发的回复数据*/
    __HAL_UART_DISABLE_IT(UART_PORT, UART_IT_IDLE);
    HAL_GPIO_WritePin(RJ45_CONFIG_PORT, RJ45_CONFIG_PIN, GPIO_PIN_RESET);
}

/*失能RJ45配置模式*/
void Disable_RJ45_Config_Mode(void)
{
    HAL_GPIO_WritePin(RJ45_CONFIG_PORT, RJ45_CONFIG_PIN, GPIO_PIN_SET);
}

/*检测TCP状态，返回0则为未连接，返回1则已连接*/
uint8_t Check_TCP_Status(void)
{
    uint8_t status ;
    status = HAL_GPIO_ReadPin(RJ45_READ_TCP_STATUS_PORT, RJ45_READ_TCP_STATUS_PIN);

    if(0 == status)
        return 1 ;

    return 0 ;
}

/*进入数据透传模式*/
uint8_t Enter_Data_Penetrate_Mode(void)
{
    /*失能配置模式*/
    Disable_RJ45_Config_Mode();
    /*使能DMA，清除数据包*/
    Enable_And_Clear_Data_Packet();
    /*开启空闲中断，此时接收的是TCP/IP协议收发的数据*/
    __HAL_UART_ENABLE_IT(UART_PORT, UART_IT_IDLE);
    Deice_Para_Config_Handledef.dataMode = 1 ;
    return 0 ;
}

/*退出数据透传模式*/
uint8_t Quit_Data_Penetrate_Mode(void)
{
    /*失能配置模式*/
    Enable_RJ45_Config_Mode();
    /*使能DMA，清除数据包*/
    Enable_And_Clear_Data_Packet();
    return 0 ;
}


/*使能DMA，清除数据包*/
static void Enable_And_Clear_Data_Packet(void)
{
    HAL_UART_DMAStop(UART_PORT);
    memset(RJ45r_Handler.RJ45TxBuffer, 0, RJ45_TXBUFFER_SIZE);
    memset(RJ45r_Handler.RJ45RxBuffer, 0, RJ45_RXBUFFER_SIZE);
    HAL_UART_Receive_DMA(UART_PORT, RJ45r_Handler.RJ45RxBuffer, RJ45_RXBUFFER_SIZE);
}

/*0 成功  其他失败*/
static uint8_t RJ45_Check_Cmd_Ack(uint8_t ack)
{
    if(RJ45r_Handler.RJ45RxBuffer[0] == ack)
        return 0;

    return 1;
}

/*SDK串口发送一个字节*/
void Uart_PutChar(uint8_t value)
{
    HAL_UART_Transmit(UART_PORT, (uint8_t *)&value, 1, 1000);
}

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 */
void uart_transmit_output(uint8_t value)
{
    Uart_PutChar(value);	                                //串口发送函数
}

/**
 * @brief  串口发送一段数据
 * @param[in] {in} 发送缓存指针
 * @param[in] {len} 数据发送长度
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
//RJ45发送网络透传数据函数，必须在透传模式下使用
void RJ45_Send_NetWork_Penetrate_Data(char* fmt, ...)
{
    if(1 == Deice_Para_Config_Handledef.dataMode)
    {
        /*调度器上锁*/
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
        /*调度器解锁*/
        #ifdef CMSIS_RTOS_SUPPORT
        rtos_sched_unlock();
        #endif
    }
}

/* 返回ch字符在sign数组中的序号 */
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

/* 十六进制数转换为十进制数 */
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
/*******************************************以上为提供给命令发送和处理的函数*********************************************/
