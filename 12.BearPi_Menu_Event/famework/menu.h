#ifndef __MENU_H
#define __MENU_H
#include "main.h"

typedef void (*menu_op_func)(uint8_t);
typedef struct OP_STRUCT
{
	int op_menu ; 				//操作菜单
	menu_op_func opfun ;	//操作方法
}OP_MENU_PAGE;


void Enter_Page(uint8_t index,uint8_t KeyValue);
void Menu_Select_Item(uint8_t current_index, uint8_t KeyValue);


void main_page_process(uint8_t Event_Code);


#endif //__MENU_H
