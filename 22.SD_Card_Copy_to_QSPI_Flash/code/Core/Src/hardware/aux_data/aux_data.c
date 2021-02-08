#include "aux_data.h"
#include "ff.h"
#include "test.h"
#include "fatfs.h"
#include "lcd_spi2_drv.h"

/*窗口句柄*/
extern WM_HWIN hItem;
//SD卡源数据路径！！
char src_dir[255] = "0:";
//要拷贝到的FLASH路径
char dst_dir[255] = FLASH_ROOT;
char flash_scan_dir[255] = FLASH_ROOT;
char sd_scan_dir[255] = SD_ROOT;


FRESULT scan_files (char* src_path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;
    char UI_Path[100] = {0};
    res = f_opendir(&dir, src_path); /* Open the directory */

    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */

            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */

            if (fno.fattrib & AM_DIR)                      /* It is a directory */
            {
                i = strlen(src_path);
                sprintf(&src_path[i], "/%s", fno.fname);
                res = scan_files(src_path);                   /* Enter the directory */

                if (res != FR_OK) break;

                src_path[i] = 0;
            }
            else
            {
                memset(UI_Path, 0, 100);
                sprintf(UI_Path, "%s/%s\n", src_path, fno.fname);
                if(strstr(fno.fname, ".bmp") != 0)
								{
                    //printf("%s", UI_Path);
										//MULTIEDIT_AddText(hItem,UI_Path);
										LCD_Clear(BLACK);
										Lcd_show_bmp(0,0,UI_Path);
										HAL_Delay(500);
								}
            }
        }
        f_closedir(&dir);
    }

    return res;
}


/*从SD卡拷贝UI文件到QSPI FLASH*/
FRESULT copy_ui_files (char* src_path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;
    char UI_Path[100] = {0};
    res = f_opendir(&dir, src_path); /* Open the directory */

    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */

            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */

            if (fno.fattrib & AM_DIR)                      /* It is a directory */
            {
                i = strlen(src_path);
                sprintf(&src_path[i], "/%s", fno.fname);
                res = scan_files(src_path);                   /* Enter the directory */

                if (res != FR_OK) break;

                src_path[i] = 0;
            }
            else
            {
                memset(UI_Path, 0, 100);
                sprintf(UI_Path, "%s/%s", src_path, fno.fname);
                if(strstr(fno.fname, ".bmp") != 0)
                    copy_file(UI_Path, "1:");
            }
						GUI_Delay(1);
        }
        f_closedir(&dir);
    }

    return res;
}


#define COPY_UNIT 4096
static BYTE read_buf[COPY_UNIT] = {0};        			/* 读缓冲区 */
static UINT real_read_num;            					  /* 文件成功读写数量 */
static UINT real_write_num;            					  /* 文件成功读写数量 */
char newfn[255];

/**
  * @brief  复制文件
  * @param  src_path:源文件路径
  * @param  dst_path:文件将要复制到的目录(不含文件名)
  * @retval result:文件系统的返回值
  */
int copy_file(char *src_path, char *dst_path)
{
    FRESULT res = FR_OK;
    char *sub_dir = NULL;
		char buf[100] = {0};
    res = f_open(&SDFile, src_path, FA_READ);

    if(res != FR_OK)
    {
        printf("打开文件失败!\n");
        return -1 ;
    }

    //获取子文件路径
    sub_dir =  strrchr(src_path, '/');

    if(NULL == sub_dir)
    {
        printf("获取子文件路径失败!\n");
        return -2 ;
    }

    //拼接成新路径
    sprintf(newfn, "%s%s", dst_path, sub_dir);
    //printf("正在复制文件 %s ...", newfn);
		sprintf(buf,"copy 0:->%s success\n",newfn);
		MULTIEDIT_AddText(hItem,buf);
    res = f_open(&USERFile, newfn, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

    if(res != FR_OK)
    {
        printf("打开文件失败!\n");
        return -3 ;
    }

    //拷贝文件
    while(res == FR_OK)
    {
        res = f_read(&SDFile, &read_buf, COPY_UNIT, &real_read_num);

        if(res != FR_OK)
        {
            printf("读取文件失败!\n");
            return -4 ;
        }

        res = f_write(&USERFile, &read_buf, real_read_num, &real_write_num);

        if(res != FR_OK)
        {
            printf("写文件失败!\n");
            return -5 ;
        }

        if(real_read_num != COPY_UNIT)
            break;
				GUI_Delay(1);
    }
    f_close(&SDFile);
    f_close(&USERFile);
    return res;
}

