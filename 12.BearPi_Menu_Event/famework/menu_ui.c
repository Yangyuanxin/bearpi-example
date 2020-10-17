#include "menu_ui.h"

Cursor Flow_Cursor ;

/*显示菜单文字项*/
void display_menu_item(Item item_struct[],uint8_t item)
{
	LCD_ShowChinese(item_struct[item].x, item_struct[item].y,
                            item_struct[item].Str, item_struct[item].front_color,
                            item_struct[item].back_color, item_struct[item].font_num, item_struct[item].mode);
}


/*显示菜单文字项*/
void display_menu_color_item(Item item_struct[],uint8_t item,uint16_t front_color)
{
	LCD_ShowChinese(item_struct[item].x, item_struct[item].y,
                            item_struct[item].Str, front_color,
                            item_struct[item].back_color, item_struct[item].font_num, item_struct[item].mode);
}

/*菜单初始化*/
void Menu_Init(void)
{
    main_page_init();
}




