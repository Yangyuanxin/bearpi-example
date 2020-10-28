#include "conf_ui.h"

#define MENU_X					70+24
#define MENU_Y					43

#define PARA_SETTING_X					70
#define PARA_SETTING_Y					73

#define ALARM_SETTING_X					70
#define ALARM_SETTING_Y					102

#define DEBUG_SETTING_X					70
#define DEBUG_SETTING_Y					131

#define DEVICE_INFO_SETTING_X		70
#define DEVICE_INFO_SETTING_Y		160

//#号字符
#define ICON_X									70-24
#define ICON_Y									73

Conf_Page conf_page_ui ;

/*配置页面菜单项显示*/
Item Conf_Item_Title_Display[] =
{
    /*显示*/
    {MENU_X, MENU_Y,  						"菜单", GREEN, BLACK, 24, 1},
    /*隐藏*/
    {MENU_X, MENU_Y,  						"菜单", BLACK, BLACK, 24, 1},
};

/*配置页面菜单项显示*/
Item Conf_Item_Display[] =
{
    /*显示*/
    {PARA_SETTING_X, PARA_SETTING_Y,  						"阈值设置", WHITE, BLACK, 24, 1},		//0		/*检测阈值===>低、中、高*/
    {ALARM_SETTING_X, ALARM_SETTING_Y,  					"报警设置", WHITE, BLACK, 24, 1},		//1		/*声音、LED、数值显示*/
    {DEBUG_SETTING_X, DEBUG_SETTING_Y,  					"调试模式", WHITE, BLACK, 24, 1},		//2		/*开、关*/
    {DEVICE_INFO_SETTING_X, DEVICE_INFO_SETTING_Y, "仪器信息", WHITE, BLACK, 24, 1},		//3		/*显示版本号、ROM、RAM等信息*/
    /*隐藏*/
    {PARA_SETTING_X, PARA_SETTING_Y,  						"阈值设置", BLACK, BLACK, 24, 1},		//4		/*检测阈值===>低、中、高*/
    {ALARM_SETTING_X, ALARM_SETTING_Y,  					"报警设置", BLACK, BLACK, 24, 1},		//5		/*声音、LED、数值显示*/
    {DEBUG_SETTING_X, DEBUG_SETTING_Y,  					"调试模式", BLACK, BLACK, 24, 1},		//6		/*开、关*/
    {DEVICE_INFO_SETTING_X, DEVICE_INFO_SETTING_Y, "仪器信息", BLACK, BLACK, 24, 1},		//7		/*显示版本号、ROM、RAM等信息*/
};

/*选择菜单项目*/
void select_conf_item(uint8_t item, uint16_t color)
{
    LCD_ShowCharStr(ICON_X, PARA_SETTING_Y, 24, "*", BLACK, BLACK, 24);
    LCD_ShowCharStr(ICON_X, ALARM_SETTING_Y, 24, "*", BLACK, BLACK, 24);
    LCD_ShowCharStr(ICON_X, DEBUG_SETTING_Y, 24, "*", BLACK, BLACK, 24);
    LCD_ShowCharStr(ICON_X, DEVICE_INFO_SETTING_Y, 24, "*", BLACK, BLACK, 24);

    switch(item)
    {
        case 0:
            LCD_ShowCharStr(ICON_X, PARA_SETTING_Y, 24, "*", BLACK, color, 24);
            break ;

        case 1:
            LCD_ShowCharStr(ICON_X, ALARM_SETTING_Y, 24, "*", BLACK, color, 24);
            break ;

        case 2:
            LCD_ShowCharStr(ICON_X, DEBUG_SETTING_Y, 24, "*", BLACK, color, 24);
            break ;

        case 3:
            LCD_ShowCharStr(ICON_X, DEVICE_INFO_SETTING_Y, 24, "*", BLACK, color, 24);
            break ;

        default:
            break ;
    }
}

//配置页面初始化
void conf_page_ui_init(uint8_t item)
{
    Flow_Cursor.flow_cursor = CONF_PAGE ;

    /*默认选择第0项*/
    conf_page_ui.select_item = item ;
    display_menu_item(Conf_Item_Title_Display, 0);

    for(int i = 0 ; i < 4 ; i++)
        display_menu_item(Conf_Item_Display, i);

    select_conf_item(conf_page_ui.select_item, RED);

    LCD_ShowChinese(20, 208, (uint8_t *)"长按右键退出", BLACK, BLACK, 32, 1);
}

//进入某一配置项
void enter_conf_item(uint8_t item)
{
    switch(item)
    {
        //阈值设置
        case 0:
            display_menu_item(Conf_Item_Title_Display, 1);

            for(int i = 4 ; i < 8 ; i++)
                display_menu_item(Conf_Item_Display, i);

            select_conf_item(conf_page_ui.select_item, BLACK);
            threshold_page_ui_init(0);
            break ;

        //报警设置
        case 1:
            display_menu_item(Conf_Item_Title_Display, 1);

            for(int i = 4 ; i < 8 ; i++)
                display_menu_item(Conf_Item_Display, i);

            select_conf_item(conf_page_ui.select_item, BLACK);
            alarm_setting_page_ui_init(0);
            break ;

        //调试模式
        case 2:
            display_menu_item(Conf_Item_Title_Display, 1);

            for(int i = 4 ; i < 8 ; i++)
                display_menu_item(Conf_Item_Display, i);

            select_conf_item(conf_page_ui.select_item, BLACK);
            debug_page_ui_init(0);
            break ;

        //仪器信息
        case 3:
            display_menu_item(Conf_Item_Title_Display, 1);

            for(int i = 4 ; i < 8 ; i++)
                display_menu_item(Conf_Item_Display, i);

            select_conf_item(conf_page_ui.select_item, BLACK);
            Version_Info_Page_Init();
            break ;

        default:
            break ;
    }
}


//配置页面按键处理
void conf_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
        //切换到下一项
        case LEFT:
            (conf_page_ui.select_item < 3) ? (conf_page_ui.select_item++) : (conf_page_ui.select_item = 0);
            select_conf_item(conf_page_ui.select_item, RED);
            break ;

        //进入某一项
        case RIGHT:
            LCD_DisplayOff();
            enter_conf_item(conf_page_ui.select_item);
            LCD_DisplayOn();
            break ;

        //退出菜单回到主菜单
        case RIGHT_LONG:
            LCD_DisplayOff();
            select_conf_item(conf_page_ui.select_item, BLACK);
            LCD_ShowChinese(20, 208, (uint8_t *)"长按右键退出", BLACK, BLACK, 32, 1);
            display_menu_item(Conf_Item_Title_Display, 1);

            for(int i = 4 ; i < 8 ; i++)
                display_menu_item(Conf_Item_Display, i);

            Flow_Cursor.flow_cursor = MAIN_PAGE ;
            Select_Main_Menu_Item(main_item);
            LCD_DisplayOn();
            break ;

        default:
            break ;
    }
}






