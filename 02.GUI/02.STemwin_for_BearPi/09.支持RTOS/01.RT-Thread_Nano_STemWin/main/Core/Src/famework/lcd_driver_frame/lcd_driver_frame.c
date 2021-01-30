#include "lcd_driver_frame.h"
#include <stdio.h>

LCD_Driver_Model lcd_model ;
void Register_Driver_Model(LCD_Driver_Model *lcd_driver_model)
{
    lcd_driver_model->lcd_driver = &lcd_driver ;
}

