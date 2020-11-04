/********************************************************************************
    * 文件名称 ：lcd.c
    * 作     者：物联网俱乐部
    * 版     本：V1.0
    * 编写日期 ：2018-4-1
    * 功     能：LCD屏幕驱动
*********************************************************************************
    * 说    明 ：本例程配套物联网俱乐部BearPi开发板使用
    *
    * 淘     宝：https://shop128001708.taobao.com/
    * 论     坛：bbs.iot-club.cn
*********************************************************************************/
#include "lcd.h"
#include "font.h"
#include "spi.h"
#include "gpio.h"
#include "Hzlib.h" 

#include "stm32l4xx_hal.h"
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];

uint16_t	POINT_COLOR = WHITE;	//画笔颜色	默认为黑色
uint16_t	BACK_COLOR 	= BLACK;	//背景颜色	默认为白色

static struct lcd_function lcd_cfg_script[] =
{
    {LCD_START, LCD_START},
    /* Sleep Out */
    {LCD_CMD, 0x11},
    {LCD_DELAY, 120},
    /* Memory Data Access Control */
    {LCD_CMD, 0x36},
    {LCD_DATA, 0x00},
    /* RGB 5-6-5-bit  */
    {LCD_CMD, 0x3a},
    {LCD_DATA, 0x65},
    /* Porch Setting */
    {LCD_CMD, 0xb2},
    {LCD_DATA, 0x0c},
    {LCD_DATA, 0x0c},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0x33},
    {LCD_DATA, 0x33},
    /*  Gate Control */
    {LCD_CMD, 0xb7},
    {LCD_DATA, 0x72},
    /* VCOM Setting */
    {LCD_CMD, 0xbb},
    {LCD_DATA, 0x3d},//Vcom=1.625V
    /* LCM Control */
    {LCD_CMD, 0xc0},
    {LCD_DATA, 0x2c},
    /* VDV and VRH Command Enable */
    {LCD_CMD, 0xc2},
    {LCD_DATA, 0x01},
    /* VRH Set */
    {LCD_CMD, 0xc3},
    {LCD_DATA, 0x19},
    /* VDV Set */
    {LCD_CMD, 0xc4},
    {LCD_DATA, 0x20},
    /* Frame Rate Control in Normal Mode */
    {LCD_CMD, 0xc6},
    {LCD_DATA, 0x01},
    /* Power Control 1 */
    {LCD_CMD, 0xd0},
    {LCD_DATA, 0xa4},
    {LCD_DATA, 0xa1},
    /* Positive Voltage Gamma Control */
    {LCD_CMD, 0xe0},
    {LCD_DATA, 0xD0},
    {LCD_DATA, 0x04},
    {LCD_DATA, 0x0D},
    {LCD_DATA, 0x11},
    {LCD_DATA, 0x13},
    {LCD_DATA, 0x2B},
    {LCD_DATA, 0x3F},
    {LCD_DATA, 0x54},
    {LCD_DATA, 0x4C},
    {LCD_DATA, 0x18},
    {LCD_DATA, 0x0D},
    {LCD_DATA, 0x0B},
    {LCD_DATA, 0x1F},
    {LCD_DATA, 0x23},
    /* Negative Voltage Gamma Control */
    {LCD_CMD, 0xe1},
    {LCD_DATA, 0xD0},
    {LCD_DATA, 0x04},
    {LCD_DATA, 0x0C},
    {LCD_DATA, 0x11},
    {LCD_DATA, 0x13},
    {LCD_DATA, 0x2C},
    {LCD_DATA, 0x3F},
    {LCD_DATA, 0x44},
    {LCD_DATA, 0x51},
    {LCD_DATA, 0x2F},
    {LCD_DATA, 0x1F},
    {LCD_DATA, 0x10},
    {LCD_DATA, 0x20},
    {LCD_CMD, 0x23},
    /* Display Inversion On */
    {LCD_CMD, 0x21},
    {LCD_CMD, 0x29},
    #if 1
    {LCD_CMD, 0x2a},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0xef},
    {LCD_CMD, 0x2b},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0x00},
    {LCD_DATA, 0xef},
    {LCD_CMD, 0x2c},
    #endif
    {LCD_END, LCD_END},
};



uint8_t SPI2_WriteByte(uint8_t* data, uint16_t size)
{
 return  HAL_SPI_Transmit(&hspi2, data, size, 0xff);
}

/**
 * @brief	LCD底层SPI发送数据函数
 *
 * @param   data	数据的起始地址
 * @param   size	发送数据大小
 *
 * @return  void
 */

static void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
	SPI2_WriteByte(data, size);
}


/**
 * @brief	写命令到LCD
 *
 * @param   cmd		需要发送的命令
 *
 * @return  void
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_DC(0);

    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief	写数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
static void LCD_Write_Data(uint8_t data)
{
    LCD_DC(1);

    LCD_SPI_Send(&data, 1);
}


/**
 * @brief	写半个字的数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
void LCD_Write_HalfWord(const uint16_t da)
{
    uint8_t data[2] = {0};

    data[0] = da >> 8;
    data[1] = da;

    LCD_DC(1);
    LCD_SPI_Send(data, 2);
}


/**
 * 设置数据写入LCD缓存区域
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    LCD_Write_Cmd(0x2C);
}

/**
 * 打开LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * 关闭LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

/*控制屏幕背光亮灭*/
void LCD_DisplayOnoff(uint8_t status) 
{
	if(0 == status)
		LCD_DisplayOff();
	else
		LCD_DisplayOn();
}

/**
 * 以一种颜色清空LCD屏
 *
 * @param   color	清屏颜色
 *
 * @return  void
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_DC(1);

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
    }
}

/**
 * 用一个颜色填充整个区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end			终点坐标
 * @param   color       		填充颜色
 *
 * @return  void
 */
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

/**
 * 画点函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_Point(uint16_t x, uint16_t y)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(POINT_COLOR);
}

/**
 * 画点带颜色函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(color);
}



/**
 * @brief	画线函数(直线、斜线)
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if(y1 == y2)
    {
        /*快速画水平线*/
        LCD_Address_Set(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = POINT_COLOR >> 8;
            lcd_buf[2 * i + 1] = POINT_COLOR;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0)incx = 1;

    else if(delta_x == 0)incx = 0;

    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;

    else if(delta_y == 0)incy = 0;

    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++)
    {
        LCD_Draw_Point(row, col);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief	画一个矩形
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/**
 * @brief	显示一个ASCII码字符
 * @param   x,y		显示起始坐标
 * @param   ch		需要显示的字符
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note	需要font.h字库文件的支持
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{
	int i = 0, j = 0;
	uint8_t temp = 0;
	uint8_t size = 0;
	uint8_t t = 0;
	
	/* 检测显示是否会越界 */
	 if((x > (LCD_Width - font_size / 2)) || (y > (LCD_Height - font_size)))	
		 return;
	
	/* 根据字符大小设置显存操作区域 */
	LCD_Address_Set(x, y, x + font_size/2 - 1, y + font_size - 1);
	 
	 /* 计算字符在字库中的偏移值*/
	 ch = ch - ' ';
	 
	 /* 显示16号/32号字体 */
	 if((font_size == 16) || (font_size == 32) )
	 {
		  /* 计算字体一个字符对应点阵集所占的字节数 */
 			size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

			for(i = 0; i < size; i++)
			{
					if(font_size == 16)
							temp = asc2_1608[ch][i];	//调用1608字体
					else if(font_size == 32)
							temp = asc2_3216[ch][i];	//调用3216字体
					else 
							return;			//没有的字库

					for(j = 0; j < 8; j++)
					{
							if(temp & 0x80)
								LCD_Write_HalfWord(font_color);
							else 
								LCD_Write_HalfWord(back_color);

							temp <<= 1;
					}
			}
	 }
	  /* 显示12号字体 */
	 else if(font_size == 12)
	 {
		  /* 计算字体一个字符对应点阵集所占的字节数 */
 			size = (font_size / 8 + ((font_size % 8) ? 1 : 0)) * (font_size / 2);

			for(i = 0; i < size; i++)
			{
				  temp = asc2_1206[ch][i];

					for(j = 0; j < 6; j++)
					{
							if(temp & 0x80)
								LCD_Write_HalfWord(font_color);
							else 
								LCD_Write_HalfWord(back_color);

							temp <<= 1;
					}
			}
	 }
	 /* 显示24号字体 */
	 else if(font_size == 24)
	 {
		  /* 计算字体一个字符对应点阵集所占的字节数 */
 			size = (font_size * 16) / 8;

			for(i = 0; i < size; i++)
			{
				  temp = asc2_2412[ch][i];
					if(i % 2 == 0)
							t = 8;
					else
							t = 4;
					for(j = 0; j < t; j++)
					{
							if(temp & 0x80)
								LCD_Write_HalfWord(font_color);
							else 
								LCD_Write_HalfWord(back_color);

							temp <<= 1;
					}
			}
	 }	 
	 /* 其余字体 */
	 else
		 return;
}
/**
 * @brief		显示一个ASCII码字符串
 * @param   x,y		显示起始坐标
 * @param   str		需要显示的字符串
 * @param   size	字体大小(支持16/24/32号字体)
 * @return  none
 * @note		1. 需要font.h字库文件的支持
 * 					2. 超过指定width不显示超过的字符
 */
void LCD_ShowCharStr(uint16_t x, uint16_t y, uint8_t max_width, char* str, uint16_t back_color, uint16_t font_color, uint8_t font_size)
{

	max_width += x;
	
	while((*str <= '~') && (*str >= ' '))	//判断是否非法字符
	{
			if(x >= max_width)
			{
					//x方向越界，结束
					break;
			}
			
			LCD_ShowChar(x,y,*str,back_color, font_color,font_size);
			x += font_size / 2;
			str++;
	}
}


/**
 * @brief	显示图片
 *
 * @remark	Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 *
 * @param   x,y		起点坐标
 * @param   width	图片宽度
 * @param   height	图片高度
 * @param   p		图片缓存数据起始地址
 *
 * @return  void
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC(1);

	if(width * height*2>65535)
	{
		LCD_SPI_Send((uint8_t *)p, 65535);
		LCD_SPI_Send((uint8_t *)(p+65535), width*height*2-65535);
	}
	else
	{
		LCD_SPI_Send((uint8_t *)p, width * height * 2);
	}
}



//配置启动参数
static void st7789_run_cfg_script(void)
{
    uint8_t data[2] = {0};
    int i = 0;
    int end_script = 0;

    do
    {
        switch (lcd_cfg_script[i].cmd)
        {
            case LCD_START:
                break;

            case LCD_CMD:
                data[0] = lcd_cfg_script[i].data ;
                LCD_Write_Cmd(data[0]);
                break;

            case LCD_DATA:
                data[0] = lcd_cfg_script[i].data ;
                LCD_Write_Data(data[0]);
                break;

            case LCD_DELAY:
                HAL_Delay(120);
                break;

            case LCD_END:
                end_script = 1;
        }

        i++;
    }
    while (!end_script);
}

/**
 * @brief	LCD初始化
 *
 * @param   void
 *
 * @return  void
 */
void LCD_Init(void)
{
	LCD_DisplayOff();
	/* 复位LCD */
    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1);
	/*使能SPI单线发送模式*/
    SPI_1LINE_TX(&hspi2);
    __HAL_SPI_ENABLE(&hspi2);
    st7789_run_cfg_script();
	LCD_Clear(BLACK);
	LCD_DisplayOn();
}



/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc); 
						else LCD_Write_Data(bc);	
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/

void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc);
						else LCD_Write_Data(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc);
						else LCD_Write_Data(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc);
						else LCD_Write_Data(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

#include <stdio.h>
/******************************************************************************
      函数说明：显示单个48x48汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese48x48(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint16_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont48)/sizeof(typFNT_GB48);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont48[k].Index[0]==*(s))&&(tfont48[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont48[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc);
						else LCD_Write_Data(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont48[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}



/******************************************************************************
      函数说明：显示单个64x64汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese64x64(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint16_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont64)/sizeof(typFNT_GB64);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont64[k].Index[0]==*(s))&&(tfont64[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont64[k].Msk[i]&(0x01<<j))LCD_Write_Data(fc);
						else LCD_Write_Data(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont64[k].Msk[i]&(0x01<<j))	LCD_Draw_ColorPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/**********************************************************/
//显示汉字
/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else if(sizey==48) LCD_ShowChinese48x48(x,y,s,fc,bc,sizey,mode);
		else if(sizey==64) LCD_ShowChinese64x64(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

