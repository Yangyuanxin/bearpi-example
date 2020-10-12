#ifndef __TEST_UI_H
#define  __TEST_UI_H
#include "main.h"

//���״̬��
#define BASE_LINE 0
#define DETECTING 1
#define DETECT_SAFETY 2
#define DETECT_DANGER 3
#define NULL_STATUS   99

typedef struct
{
	int Test_Process ;		/*���Խ���*/
	__IO uint8_t Detect_Step ; /*��ⲽ��*/
	uint8_t Start_Detect;	/*��ʼ���Ա�־*/
	uint8_t Count_Base  ; /*ͳ�ƻ�׼����*/
	uint8_t Count_Alarm ;	/*ͳ�Ʊ�������*/
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

/*��ʾ��׼1����0*/
void display_base(uint8_t enable);
/*��ʾ���1����0*/
void display_detect(uint8_t enable);
/*��ʾ��ȫ1����0*/
void display_safety(uint8_t enable);
/*��ʾΣ��1����0*/
void display_danger(uint8_t enable);

#endif //__TEST_UI_H

