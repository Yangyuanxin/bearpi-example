#ifndef __MAIN_UI_H
#define __MAIN_UI_H
#include "main.h"


extern uint8_t main_item ;
/*��ҳ���ʼ��*/
void main_page_init(void);
/*��ҳ���¼�����*/
void main_page_process(uint8_t Event_Code);
/*���л���ҳ��˵�,�ҽ���˵���*/
void Select_Main_Menu_Item(uint8_t item);

#endif //__MAIN_UI_H
