#include "sleep_ui.h"



/*待机页面初始化*/
void sleep_page_init(void)
{
    standby_mode();
}

/*待机页面事件处理*/
void sleep_page_process(uint8_t Event_Code)
{
    if(Event_Code == LEFT_LONG)
        standby_mode();
}
