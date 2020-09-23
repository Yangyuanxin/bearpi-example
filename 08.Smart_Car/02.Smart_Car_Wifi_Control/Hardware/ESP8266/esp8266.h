#ifndef __ESP8266_H
#define __ESP8266_H
#include "main.h"


/*����������󳤶�*/
#define TX_BUFF_SIZE 50
/*����������󳤶�*/
#define RX_BUFF_SIZE 1024

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


void wifi_init_printf(const char *format, ...);
//WIFI��ʼ��
void WIFI_Init(uint8_t *ssid, uint8_t *password);

#endif //__ESP8266_h

