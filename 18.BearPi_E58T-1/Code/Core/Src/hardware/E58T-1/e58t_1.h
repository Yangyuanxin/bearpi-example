#ifndef __E58T_1_H
#define __E58T_1_H
#include "main.h"


#define Send_Byte(Byte) send_byte((Byte)&0xFF)  



/*发送文本到打印机模块*/
void SendStringToPrintModule(char *str);
/*设置文本对齐方式*/
void Set_Text_Alignment(uint8_t status);


#endif //__E58T_1_H

