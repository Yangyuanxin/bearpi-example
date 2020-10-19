#include "main_ui.h"
#include "bsp_bmp.h"

//待机
#define ITEM_STANDBY_X		 			0
#define ITEM_STANDBY_Y		 			208

//菜单
#define ITEM_MENU_X		 		 		176
#define ITEM_MENU_Y		 		 		208

#define LOGO_X	55
#define LOGO_Y 	55


//检测
#define DETECT_LOGO "0:/UI/main_page/detect_logo.bmp"
//记录
#define LOG_LOGO 		"0:/UI/main_page/log_logo.bmp"
//配置
#define CONF_LOGO 		"0:/UI/main_page/conf_logo.bmp"


uint8_t main_item = 0 ;


Item Main_Item[] =
{
    /*隐藏*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, "待机", BLACK, BLACK, 32, 1},		//0
    {ITEM_MENU_X, ITEM_MENU_Y, 			 "菜单", BLACK, BLACK, 32, 1},								//1
    /*显示*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, "待机", WHITE, BLACK, 32, 1},		//2
    {ITEM_MENU_X, ITEM_MENU_Y, 			 "菜单", WHITE, BLACK, 32, 1},								//3
};


/*底部菜单栏显示*/
void	main_menu_item_display(uint8_t enable)
{
		if(enable == 1)
		{
			for(int i = 2 ; i < 4 ; i++)
          display_menu_item(Main_Item, i);
		}
		else if(enable == 0)
		{
			for(int i = 0 ; i < 2 ; i++)
            display_menu_item(Main_Item, i);
		}
}

/*显示主页面*/
void display_main_page(uint8_t enable)
{
    if(enable == 1)
    {
        main_menu_item_display(1);
    }
    else if(enable == 0)
    {
				LCD_Fill(LOGO_X,LOGO_Y,LOGO_X+128,LOGO_Y+128,BLACK);
        main_menu_item_display(0);
    }
}



/*主页面初始化*/
void main_page_init(void)
{
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    //关LCD显示
    LCD_DisplayOff();
    LCD_Draw_ColorRect(0, 35, 239, 200, WHITE);
    //显示时钟
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
            DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day, \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
           );
    LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24);
    //显示主页面菜单项
    display_main_page(1);
    Select_Main_Menu_Item(main_item);
    //开LCD显示
    LCD_DisplayOn();
}


//选择菜单项
void Select_Main_Menu_Item(uint8_t item)
{
		LCD_Fill(LOGO_X,LOGO_Y,LOGO_X+128,LOGO_Y+128,BLACK);
    switch(item)
    {
        case 0:
						Lcd_show_bmp(LOGO_X,LOGO_Y,DETECT_LOGO);
            break ;

        case 1:
						Lcd_show_bmp(LOGO_X,LOGO_Y,LOG_LOGO);
            break ;

        case 2:
						Lcd_show_bmp(LOGO_X,LOGO_Y,CONF_LOGO);
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
                    display_main_page(0);
                    test_page_init();
                    break ;

                case 1:
                    break ;

                case 2:
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

				/*长按右键*/
        case RIGHT_LONG:
						LCD_DisplayOff();
						display_main_page(0);
						conf_page_ui_init();
						LCD_DisplayOn();
            break ;

        default:
            break ;
    }
}

