#include "lcd_plot_display.h"
#include "spi.h"
#include "lcd.h"

plot_data_handler plot_handler ;
/*
cur_data:��ǰҪ��ʾ���������ݰ�
cur_data_size:��ǰҪ��ʾ���������ݰ��Ĵ�С
*/
void LCD_Plot_Remap(uint16_t *cur_data, uint16_t *backup_data, uint16_t cur_data_size)
{
		uint32_t i = 0 ;
		float temp = 0;
		/*���ݰ����ֵ*/
    uint16_t max = 0;
		/*���ݰ���Сֵ*/
    uint16_t min = 0;
		float scale = 0.0;
		uint16_t value = 0;
		float max_min_diff = 0.0;
		/*������ʾ�ĸ߶�*/
		float height = PLOT_DISPLAY_AREA_Y;
		char display_rel_buf[20] = {0};
    char display_max_buf[20] = {0};
		char display_min_buf[20] = {0};
		char display_sub_buf[20] = {0};
		/*��ʾX������*/
		for(uint8_t i = PLOT_DISPLAY_AREA_X-1 ; i < 240 ; i++)
        LCD_Draw_ColorPoint(i, 239, RED);
		/*��ʾY������*/
    for(uint8_t i = LCD_Y-PLOT_DISPLAY_AREA_Y ; i < 240 ; i++)
        LCD_Draw_ColorPoint(PLOT_DISPLAY_AREA_X-1, i, RED);

		value = 0 ;
		for(i = 0; i < cur_data_size; i++)
        if(cur_data[i] > value)
            value = cur_data[i];
		max = value ;
		value = cur_data[0];
		for(i = 0; i < cur_data_size; i++)
        if(cur_data[i] < value)
            value = cur_data[i];
		min = value ;
		
		sprintf(display_rel_buf,"%04d",cur_data[DATA_SIZE-1]);
		sprintf(display_max_buf,"%04d",max);
		sprintf(display_min_buf,"%04d",min);
		sprintf(display_sub_buf,"%04d",max-min);
		
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+10,LCD_X,16,16,"rel:");
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+20+10,LCD_X, 16, 16, display_rel_buf);
		
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+50+10,LCD_X,16,16,"max:");
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+70+10,LCD_X, 16, 16, display_max_buf);
		
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+100+10,LCD_X,16,16,"min:");
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+120+10,LCD_X, 16, 16, display_min_buf);
		
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+150+10,LCD_X,16,16,"sub:");
		LCD_ShowString(5,LCD_Y-PLOT_DISPLAY_AREA_Y+170+10,LCD_X, 16, 16, display_sub_buf);
		
    if(min > max) 
			return ;

    max_min_diff = (float)(max - min);
    scale = (float)(max_min_diff / height);

    if(cur_data_size < DATA_SIZE) 
			return;

    for(i = 0; i < DATA_SIZE; i ++)
    {
        temp = cur_data[i] - min;
        backup_data[i] =  DATA_SIZE - (uint16_t)(temp / scale) - 1;
    }
}


/*��ʾ����*/
void LCD_Plot_Display(uint16_t *pData, uint32_t size, uint16_t color)
{
    uint32_t i, j;
    uint8_t color_L = (uint8_t) color;
    uint8_t color_H = (uint8_t) (color >> 8);

    if(size < DATA_SIZE)	return ;

    for (i = PLOT_DISPLAY_AREA_X; i < DATA_SIZE - 1; i++)
    {
        if (pData[i + 1] >= pData[i])
        {
            LCD_Address_Set(i, pData[i], i, pData[i + 1]);
            LCD_DC(1);

            for (j = pData[i]; j <= pData[i + 1]; j++)
            {
                *((uint8_t*) &hspi2.Instance->DR) = color_H;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);

                *((uint8_t*) &hspi2.Instance->DR) = color_L;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);
            }
        }
        else
        {
            LCD_Address_Set(i, pData[i + 1], i, pData[i]);
            LCD_DC(1);

            for (j = pData[i + 1]; j <= pData[i]; j++)
            {
                *((uint8_t*) &hspi2.Instance->DR) = color_H;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);

                *((uint8_t*) &hspi2.Instance->DR) = color_L;

                while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) != 1);
            }
        }
    }
}

