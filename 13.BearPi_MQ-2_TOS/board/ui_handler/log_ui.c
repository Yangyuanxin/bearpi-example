#include "log_ui.h"

/*记录查询*/
#define LOG_TITLE_X  		67
#define LOG_TITLE_Y	 		40

/*序号*/
#define SERIAL_NUM_X		0
#define SERIAL_NUM_Y		90

/*检测时间*/
#define DETECT_DATETIME_X	0
#define DETECT_DATETIME_Y	135

/*检测结果*/
#define DETECT_RESULT_X		0
#define DETECT_RESULT_Y		180

/*无记录*/
#define NO_LOG_TITLE_X 		74
#define NO_LOG_TITLE_Y 		105

/*记录查询页面菜单项显示*/
Item Log_Item[] =
{
    {LOG_TITLE_X, LOG_TITLE_Y, 				"记录查询", WHITE, BLACK, 24, 1},		//0
    {SERIAL_NUM_X, SERIAL_NUM_Y, 			"序号", 	WHITE, BLACK, 24, 1},		//1
    {DETECT_DATETIME_X, DETECT_DATETIME_Y,  "检测时间", 	WHITE, BLACK, 24, 1},	//2
    {DETECT_RESULT_X, DETECT_RESULT_Y, 		"检测结果", 	WHITE, BLACK, 24, 1},	//3

    {LOG_TITLE_X, LOG_TITLE_Y, 				"记录查询", BLACK, BLACK, 24, 1},		//4
    {SERIAL_NUM_X, SERIAL_NUM_Y, 			"序号", 	BLACK, BLACK, 24, 1},		//5
    {DETECT_DATETIME_X, DETECT_DATETIME_Y,  "检测时间", 	BLACK, BLACK, 24, 1},	//6
    {DETECT_RESULT_X, DETECT_RESULT_Y, 		"检测结果", 	BLACK, BLACK, 24, 1},	//7
};

/*记录查询页面状态显示*/
Item Log_Status_Item[] =
{
    {NO_LOG_TITLE_X, NO_LOG_TITLE_Y, 		"无记录", WHITE, BLACK, 32, 1},		//0
    {NO_LOG_TITLE_X, NO_LOG_TITLE_Y, 		"无记录", BLACK, BLACK, 32, 1},		//1
};

/*显示记录页面文字*/
static void Display_log_setting_page_item_font(int item, uint16_t color)
{
    LCD_ShowChinese(Log_Item[item].x, Log_Item[item].y,
                    Log_Item[item].Str, color,
                    Log_Item[item].back_color, Log_Item[item].font_num, Log_Item[item].mode);
}

/*记录页面显示1隐藏0*/
void log_item_display(uint8_t enable)
{
    if(enable == 1)
    {
        for(int i = 0 ; i < 4 ; i++)
            Display_log_setting_page_item_font(i, WHITE);

        /*显示序号冒号*/
        LCD_ShowCharStr(SERIAL_NUM_X + 48, SERIAL_NUM_Y, 125, ":", BLACK, WHITE, 24);
        /*显示检测时间冒号*/
        LCD_ShowCharStr(DETECT_DATETIME_X + 96, DETECT_DATETIME_Y, 125, ":", BLACK, WHITE, 24);
        /*显示检测结果冒号*/
        LCD_ShowCharStr(DETECT_RESULT_X + 96, DETECT_RESULT_Y, 125, ":", BLACK, WHITE, 24);
    }
    else if(enable == 0)
    {
        for(int i = 4 ; i < 8 ; i++)
            Display_log_setting_page_item_font(i, BLACK);

        /*显示序号冒号*/
        LCD_ShowCharStr(SERIAL_NUM_X + 48, SERIAL_NUM_Y, 125, ":", BLACK, BLACK, 24);
        /*显示检测时间冒号*/
        LCD_ShowCharStr(DETECT_DATETIME_X + 96, DETECT_DATETIME_Y, 125, ":", BLACK, BLACK, 24);
        /*显示检测结果冒号*/
        LCD_ShowCharStr(DETECT_RESULT_X + 96, DETECT_RESULT_Y, 125, ":", BLACK, BLACK, 24);
    }
}

/*显示无记录*/
void display_no_log(uint8_t enable)
{
    if(enable == 1)
    {
        LCD_ShowChinese(Log_Status_Item[0].x, Log_Status_Item[0].y,
                        Log_Status_Item[0].Str, Log_Status_Item[0].front_color,
                        Log_Status_Item[0].back_color, Log_Status_Item[0].font_num, Log_Status_Item[0].mode);
    }
    else if(enable == 0)
    {
        LCD_ShowChinese(Log_Status_Item[1].x, Log_Status_Item[1].y,
                        Log_Status_Item[1].Str, Log_Status_Item[1].front_color,
                        Log_Status_Item[1].back_color, Log_Status_Item[1].font_num, Log_Status_Item[1].mode);
    }
}

/*显示序号*/
static void display_log_serial_number(int serial_number, uint8_t enable)
{
    char buf[10] = {0};
    memset(buf, 0, 10);
    sprintf(buf, "%05d", serial_number);

    if(enable == 1)
        LCD_ShowCharStr(SERIAL_NUM_X + 60, SERIAL_NUM_Y, 100, buf, BLACK, WHITE, 24);
    else if(enable == 0)
        LCD_ShowCharStr(SERIAL_NUM_X + 60, SERIAL_NUM_Y, 100, buf, BLACK, BLACK, 24);
}

/*显示检测日期和时间*/
static void display_log_detect_datetime(int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t enable)
{
    char buf[20] = {0};
    memset(buf, 0, 10);
    sprintf(buf, "%02d%02d%02d%02d%02d", year - 2000, month, day, hour, minute);

    if(enable == 1)
        LCD_ShowCharStr(DETECT_DATETIME_X + 108, DETECT_DATETIME_Y, 120, buf, BLACK, WHITE, 24);
    else if(enable == 0)
        LCD_ShowCharStr(DETECT_DATETIME_X + 108, DETECT_DATETIME_Y, 120, buf, BLACK, BLACK, 24);
}

/*显示检测结果*/
static void display_log_detect_result(uint8_t result, uint8_t enable)
{
    char buf[10] = {0};
    memset(buf, 0, 10);
		LCD_Fill(DETECT_RESULT_X+108, DETECT_RESULT_Y, DETECT_RESULT_X+108+ 48, DETECT_RESULT_Y+24, BLACK);
    if(enable == 1)
    {
        if(result == 0)
            LCD_ShowChinese(DETECT_RESULT_X + 108, DETECT_RESULT_Y,	\
                            (uint8_t *)"安全", WHITE, BLACK, 24, 1);
        else if(result == 1)
            LCD_ShowChinese(DETECT_RESULT_X + 108, DETECT_RESULT_Y,	\
                            (uint8_t *)"危险", WHITE, BLACK, 24, 1);
    }
    else if(enable == 0)
    {
        LCD_ShowChinese(DETECT_RESULT_X + 108, DETECT_RESULT_Y,	\
                        (uint8_t *)"安全", BLACK, BLACK, 24, 0);
        LCD_ShowChinese(DETECT_RESULT_X + 108, DETECT_RESULT_Y,	\
                        (uint8_t *)"危险", BLACK, BLACK, 24, 0);
    }
}

/*记录查询数据初始化*/
void log_page_data_init(void)
{
    /*读取当前检测的流水号*/
    csv_file_record.Current_Serial_Number = \
                                            User_Memory_Para.detetct_log_serial_number ;

    if(0 == csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX)
    {
        csv_file_record.file_number_index = \
                                            csv_file_record.Current_Serial_Number / DETECT_DATA_INDEX - 1 ;
    }
    else
    {
        csv_file_record.file_number_index = \
                                            csv_file_record.Current_Serial_Number / DETECT_DATA_INDEX;
    }

    /*计算当前数据项位于该文件的第几行*/
    if((csv_file_record.Current_Serial_Number % DETECT_DATA_INDEX == 0) \
            && (csv_file_record.Current_Serial_Number >= DETECT_DATA_INDEX))
    {
        csv_file_record.Current_Line = \
                                       csv_file_record.Current_Serial_Number % \
                                       DETECT_DATA_INDEX + DETECT_DATA_INDEX;
    }
    else
    {
        csv_file_record.Current_Line = \
                                       csv_file_record.Current_Serial_Number % \
                                       DETECT_DATA_INDEX ;
    }

    /*当前没有任何数据*/
    if(0 == csv_file_record.Current_Serial_Number)
    {
        DEBUG("当前无记录数据...\n");
        display_no_log(1);
    }
    /*当前有数据*/
    else
    {
        log_item_display(1);
        /*显示最新检测得到的一条数据，需要计算得到当前位于哪个文件索引，以及当前位于哪一行*/
        ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);
        /*显示记录存储项*/
        display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
        display_log_detect_datetime(csv_file_record.year, csv_file_record.month,
                                    csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
        display_log_detect_result(csv_file_record.detect_result, 1);
        /*显示记录存储项*/
    }
}

/*记录查询页面初始化*/
void log_page_init(void)
{
    Flow_Cursor.flow_cursor = LOG_PAGE ;
    log_page_data_init();
}


/*记录页面数据向前向后查找*/
void log_page_data_prev_next_handler(uint8_t KeyValue)
{
    int ret = -1 ;

    switch(KeyValue)
    {
        case LEFT:
            (csv_file_record.Current_Serial_Number > 1) ?	\
            (csv_file_record.Current_Serial_Number--) :	\
            (csv_file_record.Current_Serial_Number = 1);

            /*计算当前数据项位于该文件的索引===>整数减1*/
            if(0 == csv_file_record.Current_Serial_Number % 100)
                csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 - 1 ;
            else
                csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 ;

            /*计算当前数据项位于该文件的第几行*/
            if((csv_file_record.Current_Serial_Number % 100 == 0) && (csv_file_record.Current_Serial_Number >= 100))
                csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 + 100;
            else
                csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 ;

            ret = ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);

            if(ret > 0)
            {
                DEBUG("文件索引:%d 当前流水号:%d  位于文件的第%d行\n", csv_file_record.file_number_index,
                      csv_file_record.Current_Serial_Number, csv_file_record.Current_Line);
                display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
                display_log_detect_datetime(csv_file_record.year, csv_file_record.month,
                                            csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
                display_log_detect_result(csv_file_record.detect_result, 1);
            }

            break ;

        case RIGHT:
            (csv_file_record.Current_Serial_Number < User_Memory_Para.detetct_log_serial_number) ?	\
            (csv_file_record.Current_Serial_Number++) :	\
            (csv_file_record.Current_Serial_Number = User_Memory_Para.detetct_log_serial_number);

            /*计算当前数据项位于该文件的索引*/
            if(0 == csv_file_record.Current_Serial_Number % 100)
                csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 - 1 ;
            else
                csv_file_record.file_number_index = csv_file_record.Current_Serial_Number / 100 ;

            /*计算当前数据项位于该文件的第几行*/
            if((csv_file_record.Current_Serial_Number % 100 == 0) && (csv_file_record.Current_Serial_Number >= 100))
                csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 + 100;
            else
                csv_file_record.Current_Line = csv_file_record.Current_Serial_Number % 100 ;

            ret = ReadlLineData(csv_file_record.file_number_index, csv_file_record.Current_Line);

            if(ret > 0)
            {
                DEBUG("文件索引:%d 当前流水号:%d  位于文件的第%d行\n", csv_file_record.file_number_index,
                      csv_file_record.Current_Serial_Number, csv_file_record.Current_Line);
                display_log_serial_number(csv_file_record.Current_Serial_Number, 1);
                display_log_detect_datetime(csv_file_record.year, csv_file_record.month,
                                            csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 1);
                display_log_detect_result(csv_file_record.detect_result, 1);
            }

            break ;
    }
}


/*记录页面按键处理*/
void log_page_process(uint8_t KeyValue)
{
    switch(KeyValue)
    {
        /*向前翻记录数据*/
        case LEFT:
            if(csv_file_record.Current_Serial_Number != 0)
                log_page_data_prev_next_handler(KeyValue);

            break ;

        /*向后翻记录数据*/
        case RIGHT:
            if(csv_file_record.Current_Serial_Number != 0)
                log_page_data_prev_next_handler(KeyValue);

            break ;

        /*长按退回主页面记录选项*/
        case RIGHT_LONG:
            log_item_display(0);

            /*如果是在无记录显示的页面，退出时要隐藏选项*/
            if(csv_file_record.Current_Serial_Number == 0)
                display_no_log(0);
            /*隐藏记录数据选项*/
            else
            {
                display_log_serial_number(csv_file_record.Current_Serial_Number, 0);
                display_log_detect_datetime(csv_file_record.year, csv_file_record.month,
                                            csv_file_record.day, csv_file_record.hour, csv_file_record.minute, 0);
                display_log_detect_result(csv_file_record.detect_result, 0);
            }

            Flow_Cursor.flow_cursor = MAIN_PAGE ;
            //关LCD显示
            LCD_DisplayOff();
            //显示时钟
            Get_Date_Time();
            sprintf(DateTime_Handler_Info.DisPlay_DateBuf, "%04d-%02d-%02d %02d:%02d:%02d", \
                    DateTime_Handler_Info.year, DateTime_Handler_Info.month, DateTime_Handler_Info.day, \
                    DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, DateTime_Handler_Info.sec
                   );
            LCD_ShowCharStr(0, 5, 240, DateTime_Handler_Info.DisPlay_DateBuf, BLACK, WHITE, 24);
            Select_Main_Menu_Item(main_item);
            //开LCD显示
            LCD_DisplayOn();
            break ;

        default:
            break ;
    }
}


