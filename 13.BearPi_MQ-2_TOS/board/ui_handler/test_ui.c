#include "test_ui.h"
#include "bsp_bmp.h"


/*基准图标*/
#define B0_LOGO "0:/UI/detect_page/b0.bmp"
#define B1_LOGO "0:/UI/detect_page/b1.bmp"
#define B2_LOGO "0:/UI/detect_page/b2.bmp"

/*检测图标*/
#define D0_LOGO "0:/UI/detect_page/d0.bmp"
#define D1_LOGO "0:/UI/detect_page/d1.bmp"
#define D2_LOGO "0:/UI/detect_page/d2.bmp"

/*安全图标*/
#define R0_LOGO "0:/UI/detect_page/r0.bmp"
/*危险图标*/
#define R1_LOGO "0:/UI/detect_page/r1.bmp"

/*烟感值显示*/
#define SMOKE_X 74
#define SMOKE_Y 130 + 24


//刷新动画
void icon_reflash(uint8_t status)
{
    if(Sensor_Flow_Cursor.Detect_Step == BASE_LINE)
    {
        switch(status)
        {
            case 0:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, B0_LOGO);
                break ;

            case 1:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, B1_LOGO);
                break ;

            case 2:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, B2_LOGO);
                break ;

            default:
                break ;
        }
    }
    else if(Sensor_Flow_Cursor.Detect_Step == DETECTING)
    {
        switch(status)
        {
            case 0:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, D0_LOGO);
                break ;

            case 1:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, D1_LOGO);
                break ;

            case 2:
                Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, D2_LOGO);
                break ;

            default:
                break ;
        }
    }
}

/*显示基准1隐藏0*/
void display_base(uint8_t enable)
{
    if(enable == 1)
    {
        Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, B2_LOGO);
        LCD_ShowChinese(94, 130, (uint8_t *)"基准", WHITE, BLACK, 24, 1);
    }
    else if(enable == 0)
    {
        LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
        LCD_Fill(94, 130, 94 + 48, 130 + 24, BLACK);
    }
}

/*显示检测1隐藏0*/
void display_detect(uint8_t enable)
{
    if(enable == 1)
    {
        Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, D2_LOGO);
        LCD_ShowChinese(94, 130, (uint8_t *)"检测", WHITE, BLACK, 24, 1);
    }
    else if(enable == 0)
    {
        LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
        LCD_Fill(94, 130, 94 + 48, 130 + 24, BLACK);
    }
}

/*显示安全1隐藏0*/
void display_safety(uint8_t enable)
{
    if(enable == 1)
    {
        Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, R0_LOGO);
        LCD_ShowChinese(94, 130, (uint8_t *)"安全", GREEN, BLACK, 24, 1);
    }
    else if(enable == 0)
    {
        LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
        LCD_Fill(94, 130, 94 + 48, 130 + 24, BLACK);
    }
}

/*显示危险1隐藏0*/
void display_danger(uint8_t enable)
{
    if(enable == 1)
    {
        Lcd_show_bmp(DETECT_LOGO_X, DETECT_LOGO_Y, R1_LOGO);
        LCD_ShowChinese(94, 130, (uint8_t *)"危险", RED, BLACK, 24, 1);
    }
    else if(enable == 0)
    {
        LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
        LCD_Fill(94, 130, 94 + 48, 130 + 24, BLACK);
    }
}

/*显示进度条框*/
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

/*显示1隐藏0进度条*/
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

/*显示烟感值*/
void display_smoke_value(int smoke_value, uint16_t color, uint8_t enable)
{
    char display_buf[20] = {0};
    memset(display_buf, 0, 20);
    sprintf(display_buf, "%d%d%d%dppm", 				\
            smoke_value / 1000 % 100, smoke_value / 100 % 10, 	\
            smoke_value / 10 % 10, smoke_value % 10);

    if(enable == 1)
        LCD_ShowCharStr(SMOKE_X, SMOKE_Y, 100, display_buf, BLACK, color, 24);
    else if(enable == 0)
        LCD_ShowCharStr(SMOKE_X, SMOKE_Y, 100, display_buf, BLACK, BLACK, 24);
}


/*测试页面初始化*/
void test_page_init(void)
{
    Flow_Cursor.flow_cursor = TEST_PAGE ;
		/*传感器检测流程初始化*/
		Sensor_Detect_Init();
    /*显示基准*/
    display_base(1);
    display_smoke_value(0, GREEN, 1);
    LCD_ShowChinese(40, 208, (uint8_t *)"按右键退出", GREEN, BLACK, 32, 1);
}

/*测试页面事件处理*/
void test_page_process(uint8_t Event_Code)
{
    switch(Event_Code)
    {
        /*重新开始测试*/
        case LEFT:
						/*传感器检测流程初始化*/
						Sensor_Detect_Init();	
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            Display_Process_Bar(0, 0);
            display_smoke_value(0, BLACK, 0);
						LCD_Fill(94, 130, 94 + 48, 130 + 24, BLACK);
            /*显示基准*/
            display_base(1);
            break ;

        /*直接跳转到安全*/
        case LEFT_LONG:
            Sensor_Flow_Cursor.Detect_Step = DETECT_SAFETY ;
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            Sensor_Flow_Cursor.Count_Alarm = 0 ;
            Sensor_Flow_Cursor.Test_Process = 0 ;
            Sensor_Flow_Cursor.Start_Detect = 0 ;
            Display_Process_Bar(0, 0);
            display_smoke_value(0, BLACK, 0);
            LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
            display_safety(1);
            LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
            break ;

        /*退出*/
        case RIGHT:
						tos_knl_sched_lock();
            Sensor_Flow_Cursor.Detect_Step = NULL_STATUS ;
            mq2_sensor_interface.led_control(&mq2_sensor_interface, 0);
            mq2_sensor_interface.buzzer_control(&mq2_sensor_interface, 0);
            Sensor_Flow_Cursor.Count_Alarm = 0 ;
            Sensor_Flow_Cursor.Test_Process = 0 ;
            Sensor_Flow_Cursor.Start_Detect = 0 ;
            LCD_Fill(73, 92, 73 + 48 + 48, 92 + 48, BLACK);
            LCD_Fill(DETECT_LOGO_X, DETECT_LOGO_Y, DETECT_LOGO_X + 84, DETECT_LOGO_Y + 84, BLACK);
            LCD_DisplayOff();
            Display_Process_Bar(0, 0);
            display_smoke_value(1234, BLACK, 0);
            LCD_ShowChinese(40, 208, (uint8_t *)"按右键退出", BLACK, BLACK, 32, 1);
            Flow_Cursor.flow_cursor = MAIN_PAGE ;
            main_page_init();
            LCD_DisplayOn();
						tos_knl_sched_unlock();
            break ;

        default:
            break ;
    }
}

