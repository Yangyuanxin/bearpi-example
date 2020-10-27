#include "version_info_ui.h"

/*封装类型*/
typedef struct
{
    uint32_t value ;
    char     *Name ;
} Type ;

Type Package_Type[] =
{
    {0x00000, "LQFP64"},
    {0x00001, "WLCSP64"},
    {0x00010, "LQFP100"},
    {0x00101, "WLCSP36"},
    {0x01000, "UFQFPN32"},
    {0x01001, "LQFP32"},
    {0x01010, "UFQFPN48"},
    {0x01011, "LQFP48"},
    {0x01100, "WLCSP49"},
    {0x01101, "UFBGA64"},
    {0x01110, "UFBGA100"},
    {0x01111, "WLCSP36 with external SMPS"},
    {0x10110, "LQFP64 with external SMPS"},
};

//显示1隐藏0版本信息
void display_version_info(int enable)
{
    char id[50] = {0};
    char flash_size[20] = {0};
    char Mcu_Package[20] = {0};
    uint32_t Type_Value = 0 ;
    sprintf(id, "id:0x%x %x %x", \
            * (__IO uint32_t *)0x1FFF7590, \
            * (__IO uint32_t *)0x1FFF7590 + 0x04, \
            * (__IO uint32_t *)0x1FFF7590 + 0x08);
    sprintf(flash_size, "flash_size:%dKB", *(__IO uint16_t *)0x1FFF75E0);
    Type_Value = *(__IO uint32_t *)0x1FFF7500 & 0x1f ;

    for(int i = 0 ; i < sizeof(Package_Type) / sizeof(Package_Type[0]); i++)
    {
        if(Type_Value == Package_Type[i].value)
        {
            sprintf(Mcu_Package, "Mcu_Package:%s", Package_Type[i].Name);
            break ;
        }
    }

    if(enable == 1)
    {
        LCD_ShowCharStr(VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, id, BLACK, RED, VERSION1_TEXT_FONT);
        LCD_ShowCharStr(VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, flash_size, BLACK, RED, VERSION2_TEXT_FONT);
        LCD_ShowCharStr(VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, Mcu_Package, BLACK, RED, VERSION3_TEXT_FONT);
    }
    else if(enable == 0)
    {
        LCD_ShowCharStr(VERSION1_TEXT_START_X, VERSION1_TEXT_START_Y, 240, id, BLACK, BLACK, VERSION1_TEXT_FONT);
        LCD_ShowCharStr(VERSION2_TEXT_START_X, VERSION2_TEXT_START_Y, 240, flash_size, BLACK, BLACK, VERSION2_TEXT_FONT);
        LCD_ShowCharStr(VERSION3_TEXT_START_X, VERSION3_TEXT_START_Y, 240, Mcu_Package, BLACK, BLACK, VERSION3_TEXT_FONT);
    }
}

//版本信息页面初始化
void Version_Info_Page_Init(void)
{
    Flow_Cursor.flow_cursor = VERSION_PAGE ;
    display_version_info(1);
}

//版本信息按键操作
void version_info_page_process(uint8_t KeyValue)
{
    switch(KeyValue)
    {
        case LEFT:
        case RIGHT:
            display_version_info(0);
            conf_page_ui_init(3);
            break ;

        default:
            break ;
    }
}

