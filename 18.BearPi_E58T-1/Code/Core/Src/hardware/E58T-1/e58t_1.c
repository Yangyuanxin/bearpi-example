#include "e58t_1.h"
#include "usart.h"

__weak void send_byte(char byte)
{
    UNUSED(byte);
}


/*初始化打印机*/
void Init_Print(void)
{
	Send_Byte(0x1B);
	Send_Byte(0x40);
}

/*发送文本到打印机模块*/
void SendStringToPrintModule(char *str)
{
    while(*str)
    {
        Send_Byte(*str);
        str++;
    }
}

/*设置文本对齐方式*/
void Set_Text_Alignment(uint8_t status)
{
	Send_Byte(0x1B);
	Send_Byte(0x61);
	switch(status)
	{
		//居左
		case 0:
			Send_Byte(0x00);
			break ;
		//居中
		case 1:
			Send_Byte(0x01);
			break ;
		//居右
		case 2:
			Send_Byte(0x02);
			break ;
		default:
			break ;
	}
}

