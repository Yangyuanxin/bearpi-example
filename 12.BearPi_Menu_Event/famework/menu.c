#include "menu.h"


//�˵���������
static OP_MENU_PAGE g_opStruct[] = 
{
	{MAIN_PAGE      , main_page_process},					
};

//��ת��������Ӧ��ҳ��
static int JUMP_Table(int8_t op , uint8_t KeyValue)
{
	assert(op >= sizeof(g_opStruct)/sizeof(g_opStruct[0]));
	assert(op < 0);
	g_opStruct[op].opfun(KeyValue);
	return 0 ;
}

//�˵�ѡ����
void Menu_Select_Item(uint8_t current_index, uint8_t KeyValue)
{
	JUMP_Table(current_index , KeyValue);
}



