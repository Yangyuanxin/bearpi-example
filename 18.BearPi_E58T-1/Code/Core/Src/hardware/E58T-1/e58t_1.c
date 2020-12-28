#include "e58t_1.h"
#include "usart.h"

__weak void send_byte(char byte)
{
    UNUSED(byte);
}


/*��ʼ����ӡ��*/
void Init_Print(void)
{
	Send_Byte(0x1B);
	Send_Byte(0x40);
}

/*�����ı�����ӡ��ģ��*/
void SendStringToPrintModule(char *str)
{
    while(*str)
    {
        Send_Byte(*str);
        str++;
    }
}

/*�����ı����뷽ʽ*/
void Set_Text_Alignment(uint8_t status)
{
	Send_Byte(0x1B);
	Send_Byte(0x61);
	switch(status)
	{
		//����
		case 0:
			Send_Byte(0x00);
			break ;
		//����
		case 1:
			Send_Byte(0x01);
			break ;
		//����
		case 2:
			Send_Byte(0x02);
			break ;
		default:
			break ;
	}
}

