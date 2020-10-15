#include "version_info_ui.h"

#define VERSION1_TEXT_START_X 5
#define VERSION1_TEXT_START_Y 40
#define VERSION1_TEXT_FONT    16

#define VERSION2_TEXT_START_X 5
#define VERSION2_TEXT_START_Y 56
#define VERSION2_TEXT_FONT    16

#define VERSION3_TEXT_START_X 5
#define VERSION3_TEXT_START_Y 72
#define VERSION3_TEXT_FONT    16

#define INFO_1	"STM32L431 ROM:256K RAM:64K"
#define INFO_2	"AUTHOR:Bruce.yang"
#define INFO_3	"BearPi_Menu_Demo_20201012"

//显示1隐藏0版本信息
void display_version_info(int enable)
{
    char *info_1 = INFO_1;
    char *info_2 = INFO_2;
    char *info_3 = INFO_3;

    if(enable == 1)
    {
        LCD_ShowCharStr(VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, info_1, BLACK, RED, VERSION1_TEXT_FONT);
        LCD_ShowCharStr(VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, info_2, BLACK, RED, VERSION2_TEXT_FONT);
        LCD_ShowCharStr(VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, info_3, BLACK, RED, VERSION3_TEXT_FONT);
    }
    else if(enable == 0)
    {
        LCD_ShowCharStr(VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, info_1, BLACK, BLACK, VERSION1_TEXT_FONT);
        LCD_ShowCharStr(VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, info_2, BLACK, BLACK, VERSION2_TEXT_FONT);
        LCD_ShowCharStr(VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, info_3, BLACK, BLACK, VERSION3_TEXT_FONT);
    }
}

//版本信息页面初始化
void Version_Info_Page_Init(void)
{
    Flow_Cursor.flow_cursor = VERSION_PAGE ;
    display_version_info(1);
    LCD_ShowChinese(20, 208, (uint8_t *)"长按右键退出", GREEN, BLACK, 32, 1);
}

//版本信息按键操作
void version_info_page_process(uint8_t KeyValue)
{
    switch(KeyValue)
    {
        case LEFT:
            break ;

        case RIGHT:
            break ;

        case RIGHT_LONG:
            display_version_info(0);
            LCD_ShowChinese(20, 208, (uint8_t *)"长按右键退出", BLACK, BLACK, 32, 1);
            Flow_Cursor.flow_cursor = MAIN_PAGE ;
            //main_page_init();
						//显示左边界符号
						LCD_ShowCharStr(21, 102, 20, "<", BLACK, GREEN, 32);
						//显示主页面菜单项
						display_main_page(1);
						Select_Main_Menu_Item(main_item);
						//显示右边界符号
						LCD_ShowCharStr(203, 102, 20, ">", BLACK, GREEN, 32);
            break ;

        default:
            break ;
    }
}

