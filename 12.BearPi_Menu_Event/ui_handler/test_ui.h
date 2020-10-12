#ifndef __TEST_UI_H
#define  __TEST_UI_H
#include "main.h"

typedef struct
{
	int Test_Process ;		/*���Խ���*/
	uint8_t Start_Detect;	/*��ʼ���Ա�־*/
	uint8_t Count_Alarm ;	/*ͳ�Ʊ�������*/
	uint8_t is_Danger 	; 	/*�Ƿ�Σ��*/
}Detect_Logic;
extern Detect_Logic detect_logic ;

#define ALARM_THRESHOLD 600 //600ppm


/*����ҳ���ʼ��*/
void test_page_init(void);
/*����ҳ���¼�����*/
void test_page_process(uint8_t Event_Code);
/*��ʾ1����0������*/
void Display_Process_Bar(int Process, int enable);
/*��ʾ�̸�ֵ*/
void display_smoke_value(int smoke_value,uint16_t color,uint8_t enable);

/*��ʾ��ȫ1����0*/
void display_safety(uint8_t enable);
/*��ʾΣ��1����0*/
void display_danger(uint8_t enable);

#endif //__TEST_UI_H

