#ifndef __ESP8266_H
#define __ESP8266_H
#include "main.h"


/*����������󳤶�*/
#define TX_BUFF_SIZE 50
/*����������󳤶�*/
#define RX_BUFF_SIZE 150

/*ESP8266��Ϊ�ȵ�ʱ������*/
#define WIFI_HOT_SPOT_SSID 		"BearPi_ESP8266"
/*ESP8266��Ϊ�ȵ�ʱ������*/
#define WIFI_HOT_SPOT_PASSWORD	"12345678"
/*AP PORT*/
#define AP_PORT 8080

typedef struct
{
    /*wifi ap����״̬��*/
    uint8_t 	wifi_apr_status ;
    /*ATָ��ͻ���*/
    uint8_t 	tx_buffer[TX_BUFF_SIZE];
    /*���ջ���*/
    uint8_t   rx_buffer[RX_BUFF_SIZE];
    /*���ͱ�־*/
    uint8_t  	tx_flag ;
    /*multi_timer��ʱ�����*/
    Timer 		wifi_timer ;
    /*��ʱ������ֵ*/
    uint16_t 	wifi_timer_count ;
    /*wifi��ɱ�־*/
    uint8_t 	wifi_completed_flag ;
    /*��ʱ�ص�*/
    void (*wifi_timeout_cb)(void);
} wifi_ap_info ;



/*����WIFI*/
#define WIFI_AT_TEST     	"AT\r\n"
/*���û�رջ���*/
#define WIFI_ATE_SET	    "ATE%d\r\n"
/*����WIFIģʽ*/
#define WIFI_AT_SET_MODE 	"AT+CWMODE=%d\r\n"
/*����WIFI�ȵ�*/
#define WIFI_AT_SAP			"AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n"
/*���ö�����ģʽ*/
#define WIFI_AT_MULTPLE		"AT+CIPMUX=%d\r\n"
/*����������ģʽ*/
#define WIFI_OPEN_SMODE		"AT+CIPSERVER=%d,%d\r\n"
/*������������������Ͽ�ʱ��*/
#define WIFI_SET_STO 		"AT+CIPSTO=%d\r\n"
/*�鿴WIFI��Ϊ������ʱ�ĵ�ַ*/
#define WIFI_VIEW_ADDR		"AT+CIFSR\r\n"


/*ÿ��״̬��ִ�еĳ�ʱ��ѯʱ��*/
#define WIFI_TEST_TIMEOUT 	       			1000
#define WIFI_SET_ATE_TIMEOUT       			200
#define WIFI_SET_MODE_TIMEOUT 	   			200
#define WIFI_BUILD_AP_INFO_TIMEOUT 			4000
#define WIFI_CONFIG_MULTPLE_CONNECT_TIMEOUT 200
#define WIFI_OPEN_SERVER_MODE_TIMEOUT		1000
#define WIFI_VIEW_IPADDR_TIMEOUT			1000

/*ÿ��״̬����Ӧ�����*/
enum
{
    ITEM_WIFI_TEST = 0,
    ITEM_WIFI_SATE,
    ITEM_WIFI_SMODE,
    ITEM_WIFI_BUIAP,
    ITEM_WIFI_CMULT,
    ITEM_WIFI_OSERV,
    ITEM_WIFI_STIMO,
    ITEM_WIFI_VADDR,
    ITEM_WIFI_GDATA,
    ITEM_WIFI_ERROR = 99
};

/*ESP8266��ΪAPģʽ���г�ʼ��*/
void Init_ESP8266_AP_Mode(void);
/*Wifi��Ϊ������ʱ�ķ���*/
void ESP8266_AP_Mode_Setting(void);
/*wifi��������*/
void wifi_send_cmd(const char *format, ...);

#endif //__ESP8266_h

