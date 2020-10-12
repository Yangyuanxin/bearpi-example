#include "main_ui.h"

//待机
#define ITEM_STANDBY_X		 			0
#define ITEM_STANDBY_Y		 			208
#define ITEM_STANDBY_FONT	 			"待机"
#define ITEM_STANDBY_FONT_WIDTH  		32


//菜单
#define ITEM_MENU_X		 		 		176
#define ITEM_MENU_Y		 		 		208
#define ITEM_MENU_FONT		 			"菜单"
#define ITEM_MENU_FONT_WIDTH  			32

//测试
#define ITEM_TEST_X		 		 		73
#define ITEM_TEST_Y		 		 		92
#define ITEM_TEST_FONT		 			"测试"
#define ITEM_TEST_FONT_WIDTH  			48

//上传
#define ITEM_UPLOAD_X		 		 	73
#define ITEM_UPLOAD_Y		 		 	92
#define ITEM_UPLOAD_FONT		 		"上传"
#define ITEM_UPLOAD_FONT_WIDTH  		48

//版本
#define ITEM_VERSION_X		 		 	73
#define ITEM_VERSION_Y		 		 	92
#define ITEM_VERSION_FONT		 		"版本"
#define ITEM_VERSION_FONT_WIDTH  		48

uint8_t main_item = 0 ;


Item Main_Item[] =
{
    /*隐藏*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, ITEM_STANDBY_FONT, BLACK, BLACK, ITEM_STANDBY_FONT_WIDTH, 1},		//0
    {ITEM_MENU_X, ITEM_MENU_Y, ITEM_MENU_FONT, BLACK, BLACK, ITEM_MENU_FONT_WIDTH, 1},								//1
    {ITEM_TEST_X, ITEM_TEST_Y, ITEM_TEST_FONT, BLACK, BLACK, ITEM_TEST_FONT_WIDTH, 1},								//2
    /*显示*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, ITEM_STANDBY_FONT, WHITE, BLACK, ITEM_STANDBY_FONT_WIDTH, 1},		//3
    {ITEM_MENU_X, ITEM_MENU_Y, ITEM_MENU_FONT, WHITE, BLACK, ITEM_MENU_FONT_WIDTH, 1},								//4
    {ITEM_TEST_X, ITEM_TEST_Y, ITEM_TEST_FONT, GREEN, BLACK, ITEM_TEST_FONT_WIDTH, 1},								//5
};

Item Main_Menu_Item[] =
{
    /*隐藏*/
    {ITEM_TEST_X, ITEM_TEST_Y, ITEM_TEST_FONT, BLACK, BLACK, ITEM_TEST_FONT_WIDTH, 1},							//0
    {ITEM_UPLOAD_X, ITEM_UPLOAD_Y, ITEM_UPLOAD_FONT, BLACK, BLACK, ITEM_UPLOAD_FONT_WIDTH, 1},					//1
    {ITEM_VERSION_X, ITEM_VERSION_Y, ITEM_VERSION_FONT, BLACK, BLACK, ITEM_VERSION_FONT_WIDTH, 1},				//2
    /*显示*/
    {ITEM_TEST_X, ITEM_TEST_Y, ITEM_TEST_FONT, GREEN, BLACK, ITEM_TEST_FONT_WIDTH, 1},							//3
    {ITEM_UPLOAD_X, ITEM_UPLOAD_Y, ITEM_UPLOAD_FONT, GREEN, BLACK, ITEM_UPLOAD_FONT_WIDTH, 1},					//4
    {ITEM_VERSION_X, ITEM_VERSION_Y, ITEM_VERSION_FONT, GREEN, BLACK, ITEM_VERSION_FONT_WIDTH, 1},				//5
};

/*显示主页面*/
void display_main_page(uint8_t enable)
{
    if(enable == 1)
    {
        for(int i = 3 ; i < 6 ; i++)
        {
            LCD_ShowChinese(Main_Item[i].x, Main_Item[i].y,
                            Main_Item[i].Str, Main_Item[i].front_color,
                            Main_Item[i].back_color, Main_Item[i].font_num, Main_Item[i].mode);
        }
    }
    else if(enable == 0)
    {
        /*隐藏*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
        LCD_ShowCharStr(21, 102, 20, "<", BLACK, BLACK, 32);
        LCD_ShowCharStr(203, 102, 20, ">", BLACK, BLACK, 32);
    }
}

/*主页面初始化*/
void main_page_init(void)
{
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    //关LCD显示
    LCD_DisplayOff();
	LCD_Draw_ColorRect(0,35,239,200,WHITE);
    //显示时钟
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
            DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day, \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
           );
    LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24);
    //LCD_ShowCharStr(0, 15, 240, "......................", BLACK, YELLOW, 24);

    //显示左边界符号
    LCD_ShowCharStr(21, 102, 20, "<", BLACK, GREEN, 32);
    //显示主页面菜单项
    display_main_page(1);
    Select_Main_Menu_Item(main_item);
    //显示右边界符号
    LCD_ShowCharStr(203, 102, 20, ">", BLACK, GREEN, 32);
    //LCD_ShowCharStr(0, 185, 240, "......................", BLACK, YELLOW, 24);
    //开LCD显示
    LCD_DisplayOn();
}

//选择菜单项
void Select_Main_Menu_Item(uint8_t item)
{
    /*隐藏*/
    LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);

    switch(item)
    {
        case 0:
            LCD_ShowChinese(Main_Menu_Item[3].x, Main_Menu_Item[3].y,
                            Main_Menu_Item[3].Str, Main_Menu_Item[3].front_color,
                            Main_Menu_Item[3].back_color, Main_Menu_Item[3].font_num, Main_Menu_Item[3].mode);
            break ;

        case 1:
            LCD_ShowChinese(Main_Menu_Item[4].x, Main_Menu_Item[4].y,
                            Main_Menu_Item[4].Str, Main_Menu_Item[4].front_color,
                            Main_Menu_Item[4].back_color, Main_Menu_Item[4].font_num, Main_Menu_Item[4].mode);
            break ;

        case 2:
            LCD_ShowChinese(Main_Menu_Item[5].x, Main_Menu_Item[5].y,
                            Main_Menu_Item[5].Str, Main_Menu_Item[5].front_color,
                            Main_Menu_Item[5].back_color, Main_Menu_Item[5].font_num, Main_Menu_Item[5].mode);
            break ;

        default :
            break ;
    }
}

//左切换主页面菜单,右进入菜单项
void Select_Main_Menu(uint8_t Event_Code)
{
    switch(Event_Code)
    {
        case LEFT:
            (main_item < 2) ? (main_item++) : (main_item = 0);
            Select_Main_Menu_Item(main_item);
            break ;

        case RIGHT:
            switch(main_item)
            {
                case 0:
					/*隐藏底部显示*/
                    for(int i = 0 ; i < 2 ; i++)
                    {
                        LCD_ShowChinese(Main_Item[i].x, Main_Item[i].y,
                                        Main_Item[i].Str, Main_Item[i].front_color,
                                        Main_Item[i].back_color, Main_Item[i].font_num, Main_Item[i].mode);
                    }
					display_main_page(0);
					test_page_init();
                    break ;

                case 1:
                    break ;

                case 2:
					/*隐藏底部显示*/
                    for(int i = 0 ; i < 2 ; i++)
                    {
                        LCD_ShowChinese(Main_Item[i].x, Main_Item[i].y,
                                        Main_Item[i].Str, Main_Item[i].front_color,
                                        Main_Item[i].back_color, Main_Item[i].font_num, Main_Item[i].mode);
                    }
                    display_main_page(0);
                    Version_Info_Page_Init();
                    break ;

                default:
                    break ;
            }

            break ;

        default:
            break ;
    }
}



/*主页面事件处理*/
void main_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
        case LEFT:
            Select_Main_Menu(Event_Code);
            break ;

        case RIGHT:
            Select_Main_Menu(Event_Code);
            break ;

        case LEFT_LONG:
            sleep_page_init();
            break ;

        case RIGHT_LONG:
            break ;

        default:
            break ;
    }
}

