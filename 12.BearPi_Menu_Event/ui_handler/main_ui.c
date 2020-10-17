#include "main_ui.h"

//����
#define ITEM_STANDBY_X		 			0
#define ITEM_STANDBY_Y		 			208

//�˵�
#define ITEM_MENU_X		 		 		176
#define ITEM_MENU_Y		 		 		208

//����
#define ITEM_TEST_X		 		 		73
#define ITEM_TEST_Y		 		 		92

//�ϴ�
#define ITEM_UPLOAD_X		 		 	73
#define ITEM_UPLOAD_Y		 		 	92

//�汾
#define ITEM_VERSION_X		 		 	73
#define ITEM_VERSION_Y		 		 	92

uint8_t main_item = 0 ;


Item Main_Item[] =
{
    /*����*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, "����", BLACK, BLACK, 32, 1},		//0
    {ITEM_MENU_X, ITEM_MENU_Y, 			 "�˵�", BLACK, BLACK, 32, 1},								//1
    /*��ʾ*/
    {ITEM_STANDBY_X, ITEM_STANDBY_Y, "����", WHITE, BLACK, 32, 1},		//2
    {ITEM_MENU_X, ITEM_MENU_Y, 			 "�˵�", WHITE, BLACK, 32, 1},								//3
};

Item Main_Menu_Item[] =
{
    /*����*/
    {ITEM_TEST_X, ITEM_TEST_Y, 		 	 "���", BLACK, BLACK, 48, 1},							//0
    {ITEM_UPLOAD_X, ITEM_UPLOAD_Y, 	 "�ϴ�", BLACK, BLACK, 48, 1},					//1
    {ITEM_VERSION_X, ITEM_VERSION_Y, "�汾", BLACK, BLACK, 48, 1},				//2
    /*��ʾ*/
    {ITEM_TEST_X, ITEM_TEST_Y, 			 "���", GREEN, BLACK, 48, 1},							//3
    {ITEM_UPLOAD_X, ITEM_UPLOAD_Y,   "�ϴ�", GREEN, BLACK, 48, 1},					//4
    {ITEM_VERSION_X, ITEM_VERSION_Y, "�汾", GREEN, BLACK, 48, 1},				//5
};

/*�ײ��˵�����ʾ*/
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

/*��ʾ��ҳ��*/
void display_main_page(uint8_t enable)
{
    if(enable == 1)
    {
        for(int i = 3 ; i < 6 ; i++)
            display_menu_item(Main_Menu_Item, i);

        main_menu_item_display(1);
    }
    else if(enable == 0)
    {
        /*����*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
        LCD_ShowCharStr(21, 102, 20, "<", BLACK, BLACK, 32);
        LCD_ShowCharStr(203, 102, 20, ">", BLACK, BLACK, 32);

        main_menu_item_display(0);
    }
}

/*��ҳ���ʼ��*/
void main_page_init(void)
{
    Flow_Cursor.flow_cursor = MAIN_PAGE ;
    //��LCD��ʾ
    LCD_DisplayOff();
    LCD_Draw_ColorRect(0, 35, 239, 200, WHITE);
    //��ʾʱ��
    Get_Date_Time();
    sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
            DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day, \
            DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
           );
    LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24);
    //��ʾ��߽����
    LCD_ShowCharStr(21, 102, 20, "<", BLACK, GREEN, 32);
    //��ʾ��ҳ��˵���
    display_main_page(1);
    Select_Main_Menu_Item(main_item);
    //��ʾ�ұ߽����
    LCD_ShowCharStr(203, 102, 20, ">", BLACK, GREEN, 32);
    //��LCD��ʾ
    LCD_DisplayOn();
}


//ѡ��˵���
void Select_Main_Menu_Item(uint8_t item)
{
    /*����*/
    LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);

    switch(item)
    {
        case 0:
            display_menu_item(Main_Menu_Item, 3);
            break ;

        case 1:
            display_menu_item(Main_Menu_Item, 4);
            break ;

        case 2:
            display_menu_item(Main_Menu_Item, 5);
            break ;

        default :
            break ;
    }
}

//���л���ҳ��˵�,�ҽ���˵���
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



/*��ҳ���¼�����*/
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

				/*�����Ҽ�*/
        case RIGHT_LONG:
						display_main_page(0);
						conf_page_ui_init();
            break ;

        default:
            break ;
    }
}

