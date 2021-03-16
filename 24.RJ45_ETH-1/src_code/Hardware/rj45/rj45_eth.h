#ifndef __RJ45_ETH_H
#define __RJ45_ETH_H
#include "main.h"


#define UART_NNUM						USART3
#define UART_PORT 					&huart3
#define RJ45_CONFIG_PORT		GPIOC
#define RJ45_CONFIG_PIN	    GPIO_PIN_9

#define RJ45_READ_TCP_STATUS_PORT			GPIOA
#define RJ45_READ_TCP_STATUS_PIN	    GPIO_PIN_8

#define RJ45_RXBUFFER_SIZE 1024
#define RJ45_TXBUFFER_SIZE 1024

#define NR_RJ45(x)  (sizeof(x)/sizeof(x[0]))
#define Delay_ms(x)	HAL_Delay(x)
#define ACK_OK 0
#define ACK_TIMEOUT 1

typedef struct
{
    __IO uint8_t  BufferReady ;
    uint8_t  RJ45TxBuffer[RJ45_TXBUFFER_SIZE];
    uint8_t  RJ45RxBuffer[RJ45_RXBUFFER_SIZE];
} RJ45HandleTypeDef;
extern RJ45HandleTypeDef RJ45r_Handler ;

typedef struct _DEVICEPORT_CONFIG
{
    uint8_t  dataMode;		/* 数据模式：0:命令模式 1:透传模式*/
    uint8_t  bNetMode;    /* 网络工作模式: 0: TCP SERVER;1: TCP CLENT; 2: UDP SERVER 3：UDP CLIENT; */
    uint8_t  gDesIP[4];   /* 目的IP地址 */
    uint16_t gNetPort;    /* 目的端口号 */
    uint8_t  bMacAddr[4];	/* 芯片MAC地址*/
    __IO uint8_t tcp_status ;	/*服务器连接状态*/
} DevicePortConfigS;
extern DevicePortConfigS Deice_Para_Handledef ;


/**********************写指令函数*************************/
/*使能RJ45配置模式*/
void Enable_RJ45_Config_Mode(void);
/*RJ45设置模式*/
uint8_t RJ45_Set_Mode(uint8_t mode, uint16_t delay_ms);
/*设置模块目的端口号*/
uint8_t Set_Module_Gobal_Port_Number(uint16_t number, uint16_t delay_ms);
/*RJ45设置目标IP*/
uint8_t Set_Module_Gobal_Ipaddr(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint16_t delay_ms);

/*更新配置参数到EEPROM*/
uint8_t Update_Config_Para_To_EEPROM(uint16_t delay_ms);
/*执行配置参数*/
uint8_t Runing_Config_Para_To_EEPROM(uint16_t delay_ms);
/*配置RJ45模块参数*/
uint8_t Config_RJ45_Module_Para(void);
/**********************写指令函数*************************/

/**********************读指令函数*************************/
/*获取芯片工作模式*/
void Get_RJ45_Chip_Work_Mode(uint16_t delay_ms);
/*获取芯片目的IP地址*/
void Get_RJ45_Chip_Gobal_Ipaddr(uint16_t delay_ms);
/*获取芯片目的端口号*/
void Get_RJ45_Chip_Gobal_Port_Number(uint16_t delay_ms);
/*获取芯片Mac地址*/
void Get_RJ45_Chip_Mac_Addr(uint16_t delay_ms);
/*获取RJ45模块参数*/
uint8_t Get_RJ45_Module_Config_Para(void);
/**********************读指令函数*************************/

/*使能RJ45配置模式*/
void Enable_RJ45_Config_Mode(void);
/*失能RJ45配置模式*/
void Disable_RJ45_Config_Mode(void);
/*检测TCP状态，返回1则为未连接，返回0则已连接*/

/*进入数据透传模式*/
uint8_t Enter_Data_Penetrate_Mode(void);
/*退出数据透传模式*/
uint8_t Quit_Data_Penetrate_Mode(void);
//RJ45发送网络透传数据函数，必须在透传模式下使用
void RJ45_Send_NetWork_Penetrate_Data(char* fmt, ...);
uint8_t Check_TCP_Status(void);

#endif //__RJ45_ETH_H

