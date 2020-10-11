#ifndef __MENU_UI_H
#define __MENU_UI_H
#include "main.h"

//�˵�������ֵ����
#define LEFT          		1
#define LEFT_LONG 				2
#define RIGHT   					3
#define RIGHT_LONG    		4


/*ȫ�ֲ˵���ʾ���*/
typedef struct 
{
	uint8_t x;
	uint8_t y;
	uint8_t Str[10];
	uint16_t front_color;
	uint16_t back_color;
	uint8_t font_num;
	uint8_t mode ;
}Item;

typedef struct
{
    /*��������α�*/
    uint8_t flow_cursor ;
} Cursor ;
extern Cursor Flow_Cursor ;

/*��ǰ�˵�*/
typedef enum 
{
	MAIN_PAGE = 0,
}OP_PAGE;

/*�˵���ʼ��*/
void Menu_Init(void);
/*��ҳ���¼�����*/
void main_page_process(uint8_t Event_Code);

#endif //__MENU_UI_H

