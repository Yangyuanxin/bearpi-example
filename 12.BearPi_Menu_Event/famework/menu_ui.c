#include "menu_ui.h"

Cursor Flow_Cursor ;

//����
#define ITEM_STANDBY_X		 			 0
#define ITEM_STANDBY_Y		 			 208
#define ITEM_STANDBY_FONT	 			 "����"
#define ITEM_STANDBY_FONT_WIDTH  32


//�˵�
#define ITEM_MENU_X		 		 			176
#define ITEM_MENU_Y		 		 			208
#define ITEM_MENU_FONT		 			"�˵�"
#define ITEM_MENU_FONT_WIDTH  	32

//����
#define ITEM_TEST_X		 		 			73
#define ITEM_TEST_Y		 		 			92
#define ITEM_TEST_FONT		 			"����"
#define ITEM_TEST_FONT_WIDTH  	48

Item Main_Item[] = 
{
	/*����*/
	{ITEM_STANDBY_X,ITEM_STANDBY_Y, ITEM_STANDBY_FONT,BLACK,BLACK,ITEM_STANDBY_FONT_WIDTH,1},		//0
	{ITEM_MENU_X,ITEM_MENU_Y,ITEM_MENU_FONT,BLACK,BLACK,ITEM_MENU_FONT_WIDTH,1},								//1
	{ITEM_TEST_X,ITEM_TEST_Y,ITEM_TEST_FONT,BLACK,BLACK,ITEM_TEST_FONT_WIDTH,1},								//2
	/*��ʾ*/	
	{ITEM_STANDBY_X,ITEM_STANDBY_Y, ITEM_STANDBY_FONT,PINK,BLACK,ITEM_STANDBY_FONT_WIDTH,1},		//3
	{ITEM_MENU_X,ITEM_MENU_Y,ITEM_MENU_FONT,PINK,BLACK,ITEM_MENU_FONT_WIDTH,1},								//4
	{ITEM_TEST_X,ITEM_TEST_Y,ITEM_TEST_FONT,GREEN,BLACK,ITEM_TEST_FONT_WIDTH,1},								//5
};

/*��ʾ��ҳ��*/
void display_main_page(uint8_t enable)
{
	if(enable == 1)
	{
		for(int i = 3 ; i < 6 ;i++)
		{
			LCD_ShowChinese(Main_Item[i].x,Main_Item[i].y,
											Main_Item[i].Str,Main_Item[i].front_color,
											Main_Item[i].back_color,Main_Item[i].font_num,Main_Item[i].mode);
		}
	}
	else if(enable == 0)
	{
		for(int i = 0 ; i < 3 ;i++)
		{
			LCD_ShowChinese(Main_Item[i].x,Main_Item[i].y,
											Main_Item[i].Str,Main_Item[i].front_color,
											Main_Item[i].back_color,Main_Item[i].font_num,Main_Item[i].mode);
		}
	}
}

/*�˵���ʼ��*/
void Menu_Init(void)
{
	Flow_Cursor.flow_cursor = MAIN_PAGE ;
	//��LCD��ʾ
	LCD_DisplayOff();
	//��ʾʱ��
	Get_Date_Time();
  sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
					DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day,\
					DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
	);
  LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, RED, 24);
	LCD_ShowCharStr(0, 15, 240, "....................", BLACK, YELLOW, 24);							 
	//��ʾ��߽����
	LCD_ShowCharStr(21, 102, 20, "<", BLACK, BLUE, 32);
	//��ʾ��ҳ��˵���
	display_main_page(1);
	//��ʾ�ұ߽����
	LCD_ShowCharStr(203, 102, 20, ">", BLACK, BLUE, 32);
	LCD_ShowCharStr(0, 185, 240, "....................", BLACK, YELLOW, 24);
	//��LCD��ʾ
	LCD_DisplayOn();
}

//����ģʽ
void standby_mode(void)
{	
	static uint8_t status = 0;
	switch(status)
	{
		case 0:
			//��ָʾ��
			HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_SET);
			//����
			LCD_DisplayOff();
			status = 1 ;
			break ;
		case 1:
			//��ָʾ��
			HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET);
			//����
			LCD_DisplayOn();
			status = 0 ;
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
			break ;
		case RIGHT:
			break ;
		case LEFT_LONG:
			standby_mode();
			break ;
		case RIGHT_LONG:
			break ;
		default:
			break ;
	}
}
