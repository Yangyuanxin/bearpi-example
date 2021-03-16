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
    uint8_t  dataMode;		/* ����ģʽ��0:����ģʽ 1:͸��ģʽ*/
    uint8_t  bNetMode;    /* ���繤��ģʽ: 0: TCP SERVER;1: TCP CLENT; 2: UDP SERVER 3��UDP CLIENT; */
    uint8_t  gDesIP[4];   /* Ŀ��IP��ַ */
    uint16_t gNetPort;    /* Ŀ�Ķ˿ں� */
    uint8_t  bMacAddr[4];	/* оƬMAC��ַ*/
    __IO uint8_t tcp_status ;	/*����������״̬*/
} DevicePortConfigS;
extern DevicePortConfigS Deice_Para_Handledef ;


/**********************дָ���*************************/
/*ʹ��RJ45����ģʽ*/
void Enable_RJ45_Config_Mode(void);
/*RJ45����ģʽ*/
uint8_t RJ45_Set_Mode(uint8_t mode, uint16_t delay_ms);
/*����ģ��Ŀ�Ķ˿ں�*/
uint8_t Set_Module_Gobal_Port_Number(uint16_t number, uint16_t delay_ms);
/*RJ45����Ŀ��IP*/
uint8_t Set_Module_Gobal_Ipaddr(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint16_t delay_ms);

/*�������ò�����EEPROM*/
uint8_t Update_Config_Para_To_EEPROM(uint16_t delay_ms);
/*ִ�����ò���*/
uint8_t Runing_Config_Para_To_EEPROM(uint16_t delay_ms);
/*����RJ45ģ�����*/
uint8_t Config_RJ45_Module_Para(void);
/**********************дָ���*************************/

/**********************��ָ���*************************/
/*��ȡоƬ����ģʽ*/
void Get_RJ45_Chip_Work_Mode(uint16_t delay_ms);
/*��ȡоƬĿ��IP��ַ*/
void Get_RJ45_Chip_Gobal_Ipaddr(uint16_t delay_ms);
/*��ȡоƬĿ�Ķ˿ں�*/
void Get_RJ45_Chip_Gobal_Port_Number(uint16_t delay_ms);
/*��ȡоƬMac��ַ*/
void Get_RJ45_Chip_Mac_Addr(uint16_t delay_ms);
/*��ȡRJ45ģ�����*/
uint8_t Get_RJ45_Module_Config_Para(void);
/**********************��ָ���*************************/

/*ʹ��RJ45����ģʽ*/
void Enable_RJ45_Config_Mode(void);
/*ʧ��RJ45����ģʽ*/
void Disable_RJ45_Config_Mode(void);
/*���TCP״̬������1��Ϊδ���ӣ�����0��������*/

/*��������͸��ģʽ*/
uint8_t Enter_Data_Penetrate_Mode(void);
/*�˳�����͸��ģʽ*/
uint8_t Quit_Data_Penetrate_Mode(void);
//RJ45��������͸�����ݺ�����������͸��ģʽ��ʹ��
void RJ45_Send_NetWork_Penetrate_Data(char* fmt, ...);
uint8_t Check_TCP_Status(void);

#endif //__RJ45_ETH_H

