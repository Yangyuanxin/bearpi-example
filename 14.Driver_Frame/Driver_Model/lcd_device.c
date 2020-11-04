#include "lcd_device.h"
#include "lcd.h"


LCD_Device  lcd_device =
{
	.lcd_init              =  LCD_Init,
    .lcd_fill		       =  LCD_Fill,
    .lcd_clear 		       =  LCD_Clear,
    .lcd_display_onoff     =  LCD_DisplayOnoff,

    .lcd_draw_rect         =  LCD_DrawRectangle,
    .lcd_draw_line     	   =  LCD_DrawLine,

    .lcd_show_image        =  LCD_Show_Image,
    .lcd_show_ascii_str    =  LCD_ShowCharStr,
    .lcd_show_chinese_str  =  LCD_ShowChinese,
};
