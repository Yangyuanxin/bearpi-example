#ifndef __VERSION_INFO_UI_H
#define __VERSION_INFO_UI_H
#include "main.h"

#define VERSION1_TEXT_START_X 5
#define VERSION1_TEXT_START_Y 40
#define VERSION1_TEXT_FONT    16

#define VERSION2_TEXT_START_X 5
#define VERSION2_TEXT_START_Y 56
#define VERSION2_TEXT_FONT    16

#define VERSION3_TEXT_START_X 5
#define VERSION3_TEXT_START_Y 72
#define VERSION3_TEXT_FONT    16

//版本信息页面初始化
void Version_Info_Page_Init(void);
//版本信息按键操作
void version_info_page_process(uint8_t KeyValue);



#endif //__VERSION_INFO_UI_H
