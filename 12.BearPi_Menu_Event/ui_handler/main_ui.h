#ifndef __MAIN_UI_H
#define __MAIN_UI_H
#include "main.h"


extern uint8_t main_item ;
/*主页面初始化*/
void main_page_init(void);
/*主页面事件处理*/
void main_page_process(uint8_t Event_Code);
/*左切换主页面菜单,右进入菜单项*/
void Select_Main_Menu_Item(uint8_t item);

/*底部菜单栏显示*/
void	main_menu_item_display(uint8_t enable);
/*显示主页面*/
void display_main_page(uint8_t enable);

#endif //__MAIN_UI_H
