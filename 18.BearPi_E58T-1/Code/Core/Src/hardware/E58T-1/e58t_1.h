#ifndef __E58T_1_H
#define __E58T_1_H
#include "main.h"


#define Send_Byte(Byte) send_byte((Byte)&0xFF)  



/*�����ı�����ӡ��ģ��*/
void SendStringToPrintModule(char *str);
/*�����ı����뷽ʽ*/
void Set_Text_Alignment(uint8_t status);


#endif //__E58T_1_H

