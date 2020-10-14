#include "test_ui.h"

#define SMOKE_X 66
#define SMOKE_Y 140

Detect_Logic detect_logic ;

//����ͼ��
#define AMI_X										115
#define AMI_Y										60
#define ITEM_AMI_FONT_WIDTH  		32

//��׼
#define ITEM_BASE_X		 		 			73
#define ITEM_BASE_Y		 		 			92
#define ITEM_BASE_FONT		 			"��׼"
#define ITEM_BASE_FONT_WIDTH  	48

//�����
#define ITEM_DETECT_X		 		 			73
#define ITEM_DETECT_Y		 		 			92
#define ITEM_DETECT_FONT		 			"���"
#define ITEM_DETECT_FONT_WIDTH  	48


//��ȫ
#define ITEM_SAFE_X		 		 		73
#define ITEM_SAFE_Y		 		 		92
#define ITEM_SAFE_FONT		 			"��ȫ"
#define ITEM_SAFE_FONT_WIDTH  			48

//Σ��
#define ITEM_DANGER_X		 		 	73
#define ITEM_DANGER_Y		 		 	92
#define ITEM_DANGER_FONT		 		"Σ��"
#define ITEM_DANGER_FONT_WIDTH  		48

Item Test_Item[] =
{
    /*��ʾ*/
    {ITEM_BASE_X, ITEM_BASE_Y, ITEM_BASE_FONT, GREEN, BLACK, ITEM_BASE_FONT_WIDTH, 1},								  //0
    {ITEM_DETECT_X, ITEM_DETECT_Y, ITEM_DETECT_FONT, GREEN, BLACK, ITEM_DETECT_FONT_WIDTH, 1},					//1
    {ITEM_SAFE_X, ITEM_SAFE_Y, ITEM_SAFE_FONT, GREEN, BLACK, ITEM_SAFE_FONT_WIDTH, 1},									//2
    {ITEM_DANGER_X, ITEM_DANGER_Y, ITEM_DANGER_FONT, RED, BLACK, ITEM_DANGER_FONT_WIDTH, 1},						//3
};


//ˢ�¶���
void icon_reflash(uint8_t status)
{
    switch(status)
    {
        case 0:
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "/", BLACK, GREEN, ITEM_AMI_FONT_WIDTH);
            break ;

        case 1:
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "-", BLACK, GREEN, ITEM_AMI_FONT_WIDTH);
            break ;

        case 2:
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "\\", BLACK, GREEN, ITEM_AMI_FONT_WIDTH);
            break ;

        case 3:
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "-", BLACK, GREEN, ITEM_AMI_FONT_WIDTH);
            break ;

        case 4:
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "/", BLACK, BLACK, ITEM_AMI_FONT_WIDTH);
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "-", BLACK, BLACK, ITEM_AMI_FONT_WIDTH);
            LCD_ShowCharStr(AMI_X, AMI_Y, 100, "\\", BLACK, BLACK, ITEM_AMI_FONT_WIDTH);
            break ;

        default:
            break ;
    }
}

/*��ʾ��׼1����0*/
void display_base(uint8_t enable)
{
    if(enable == 1)
    {
        LCD_ShowChinese(Test_Item[0].x, Test_Item[0].y,
                        Test_Item[0].Str, Test_Item[0].front_color,
                        Test_Item[0].back_color, Test_Item[0].font_num, Test_Item[0].mode);
    }
    else if(enable == 0)
    {
        /*����*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
    }
}

/*��ʾ���1����0*/
void display_detect(uint8_t enable)
{
    if(enable == 1)
    {
        LCD_ShowChinese(Test_Item[1].x, Test_Item[1].y,
                        Test_Item[1].Str, Test_Item[1].front_color,
                        Test_Item[1].back_color, Test_Item[1].font_num, Test_Item[1].mode);
    }
    else if(enable == 0)
    {
        /*����*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
    }
}

/*��ʾ��ȫ1����0*/
void display_safety(uint8_t enable)
{
    if(enable == 1)
    {
        LCD_ShowChinese(Test_Item[2].x, Test_Item[2].y,
                        Test_Item[2].Str, Test_Item[2].front_color,
                        Test_Item[2].back_color, Test_Item[2].font_num, Test_Item[2].mode);
    }
    else if(enable == 0)
    {
        /*����*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
    }
}

/*��ʾΣ��1����0*/
void display_danger(uint8_t enable)
{
    if(enable == 1)
    {
        LCD_ShowChinese(Test_Item[3].x, Test_Item[3].y,
                        Test_Item[3].Str, Test_Item[3].front_color,
                        Test_Item[3].back_color, Test_Item[3].font_num, Test_Item[3].mode);
    }
    else if(enable == 0)
    {
        /*����*/
        LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
    }
}

/*��ʾ��������*/
void Display_Process_Bar_Frame(int enable)
{
    if(enable == 1)
    {
        LCD_Draw_ColorRect(10, 180, 230, 190, BLUE);
        LCD_Fill(12, 182, 228, 188, BLACK);
    }
    else if(enable == 0)
    {
        LCD_Draw_ColorRect(10, 180, 230, 190, BLACK);
        LCD_Fill(12, 182, 228, 188, BLACK);
    }
}

/*��ʾ1����0������*/
void Display_Process_Bar(int Process, int enable)
{
    uint8_t buf[] = {20, 40, 80, 100, 120, 140, 160, 180, 200, 228};

    if(enable == 1)
    {
        LCD_Fill(12, 182, buf[Process / 10], 188, GREEN);
    }
    else if(enable == 0)
    {
        Display_Process_Bar_Frame(enable);
        LCD_Fill(12, 182, buf[9], 188, BLACK);
    }
}

/*��ʾ�̸�ֵ*/
void display_smoke_value(int smoke_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    sprintf(display_buf, "%d%d%d%dppm", 				\
            smoke_value / 1000 % 100, smoke_value / 100 % 10, 	\
            smoke_value / 10 % 10, smoke_value % 10);

    if(enable == 1)
        LCD_ShowCharStr(SMOKE_X, SMOKE_Y, 100, display_buf, BLACK, color, 32);
    else if(enable == 0)
        LCD_ShowCharStr(SMOKE_X, SMOKE_Y, 100, display_buf, BLACK, BLACK, 32);
}


/*����ҳ���ʼ��*/
void test_page_init(void)
{
    Flow_Cursor.flow_cursor = TEST_PAGE ;
    detect_logic.Test_Process = 0 ;
    detect_logic.Start_Detect = 1 ;
    /*���״̬����Ϊ��׼*/
    detect_logic.Detect_Step = BASE_LINE ;
    detect_logic.Count_Base = 0 ;
    /*��ʾ��׼*/
    display_base(1);
    display_smoke_value(0, GREEN, 1);
    LCD_ShowChinese(40, 208, (uint8_t *)"���Ҽ��˳�", GREEN, BLACK, 32, 1);
}

/*����ҳ���¼�����*/
void test_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
        /*���¿�ʼ����*/
        case LEFT:
            detect_logic.Detect_Step = BASE_LINE ;
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            detect_logic.Count_Alarm = 0 ;
            detect_logic.Test_Process = 0 ;
            detect_logic.Start_Detect = 1 ;
            Display_Process_Bar(0, 0);
            display_smoke_value(0, BLACK, 0);
            LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
            detect_logic.Count_Base = 0 ;
            /*��ʾ��׼*/
            display_base(1);
            break ;

        /*ֱ����ת����ȫ*/
        case LEFT_LONG:
            detect_logic.Detect_Step = DETECT_SAFETY ;
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            detect_logic.Count_Alarm = 0 ;
            detect_logic.Test_Process = 0 ;
            detect_logic.Start_Detect = 0 ;
            Display_Process_Bar(0, 0);
            display_smoke_value(0, BLACK, 0);
            LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
            display_safety(1);
            icon_reflash(4);
            break ;

        /*�˳�*/
        case RIGHT:
            detect_logic.Detect_Step = NULL_STATUS ;
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            detect_logic.Count_Alarm = 0 ;
            detect_logic.Test_Process = 0 ;
            detect_logic.Start_Detect = 0 ;
            LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
            icon_reflash(4);
            LCD_DisplayOff();
            Display_Process_Bar(0, 0);
            display_smoke_value(0, BLACK, 0);
            LCD_ShowChinese(40, 208, (uint8_t *)"���Ҽ��˳�", BLACK, BLACK, 32, 1);
            Flow_Cursor.flow_cursor = MAIN_PAGE ;
            main_page_init();
            LCD_DisplayOn();
            break ;

        default:
            break ;
    }
}

