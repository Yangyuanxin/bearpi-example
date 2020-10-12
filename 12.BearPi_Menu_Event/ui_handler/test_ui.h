#ifndef __TEST_UI_H
#define  __TEST_UI_H
#include "main.h"

//检测状态机
#define BASE_LINE 0
#define DETECTING 1
#define DETECT_SAFETY 2
#define DETECT_DANGER 3
#define NULL_STATUS   99

typedef struct
{
	int Test_Process ;		/*测试进度*/
	__IO uint8_t Detect_Step ; /*检测步骤*/
	uint8_t Start_Detect;	/*开始测试标志*/
	uint8_t Count_Base  ; /*统计基准次数*/
	uint8_t Count_Alarm ;	/*统计报警次数*/
}Detect_Logic;
extern Detect_Logic detect_logic ;

#define ALARM_THRESHOLD 600 //600ppm


/*测试页面初始化*/
void test_page_init(void);
/*测试页面事件处理*/
void test_page_process(uint8_t Event_Code);
/*显示1隐藏0进度条*/
void Display_Process_Bar(int Process, int enable);
/*显示烟感值*/
void display_smoke_value(int smoke_value,uint16_t color,uint8_t enable);

/*显示基准1隐藏0*/
void display_base(uint8_t enable);
/*显示检测1隐藏0*/
void display_detect(uint8_t enable);
/*显示安全1隐藏0*/
void display_safety(uint8_t enable);
/*显示危险1隐藏0*/
void display_danger(uint8_t enable);

#endif //__TEST_UI_H

