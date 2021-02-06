#include "test.h"
#include "fatfs.h"

uint32_t count = 0;
uint8_t read_buf[50] = {0};
/*挂载文件系统*/
uint8_t work[_MAX_SS] = {0} ;
uint8_t write_buf[50] = "hello BearPi\r\n";


/*挂载SD卡*/
int Mount_SD_Fatfs(void)
{
    /*挂载SD卡*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    printf("SD卡文件系统挂载成功!\n");
    return 0 ;
}

/*挂载QSPI_FLASH的fatfs文件系统*/
void Mount_QSPI_FLASH_Fatfs(void)
{
    retUSER = f_mount(&USERFatFS, USERPath, 1);

    if(retUSER != FR_OK)
    {
        if(retUSER == FR_NO_FILESYSTEM)
        {
            printf("f_mount 没有文件系统,开始格式化spi-flash\r\n");
            retUSER = f_mkfs(USERPath, FM_ANY, 0, work, sizeof(work));

            if(retUSER != FR_OK)
            {
                printf("f_mkfs 格式化失败，err = %d\r\n", retUSER);
            }
            else
            {
                printf("格式化成功，开始重新挂载spi-flash\r\n");
                retUSER = f_mount(&USERFatFS, USERPath, 1);

                if(retUSER != FR_OK)
                {
                    printf("f_mount 发生错误，err = %d\r\n", retUSER);
                }
                else
                {
                    printf("spi-flash文件系统挂载成功\r\n");
                }
            }
        }
        else
        {
            printf("f_mount 发生其他错误，err = %d\r\n", retUSER);
        }
    }
    else
        printf("qspi文件系统挂载成功\r\n");
}

/*获取盘内存*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);//得到磁盘信息及空闲簇数量

    if(res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize; //得到总扇区数
        fre_sect = fre_clust * fs1->csize;        //得到空闲扇区数
        #if _MAX_SS!=512                                  //扇区大小不是512字节,则转换为512字节
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
        #endif
        *total = tot_sect >> 1; //单位为KB
        *free = fre_sect >> 1; //单位为KB
    }

    return res;
}

void test_get_sd_card_cap(void)
{
    uint32_t Total = 0; //读取SD卡总容量
    uint32_t Free = 0; //读取SD卡剩余容量
    f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //获取SD卡总容量和剩余容量
    printf("当前Fatfs总容量:%dKB==>%dMB 剩余容量:%dKB==>%dMB\n", Total, Total / 1024, Free, Free / 1024);
}

void test_get_qspi_cap(void)
{
    uint32_t Total = 0; //读取FLASH总容量
    uint32_t Free = 0; //读取FLASH剩余容量
    f_GetTotal_Free((uint8_t*)"1:", &Total, &Free); //获取Fatfs总容量和剩余容量
    printf("当前Fatfs总容量:%dKB==>%dMB 剩余容量:%dKB==>%dMB\n", Total, Total / 1024, Free, Free / 1024);
}


//测试SD卡挂载的Fatfs文件系统
void test_sd_card_fatfs(void)
{
    uint8_t res ;
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    printf("\r\n****** 即将进行SD卡Fatfs文件写入测试... ******\r\n");
    res = f_open(&SDFile, "0:BearPi_SD.txt", FA_OPEN_ALWAYS | FA_WRITE);

    if(res == FR_OK)
    {
        printf("打开/创建BearPi_SD文件成功，向文件写入数据。\r\n");
        res = f_write(&SDFile, write_buf, strlen((const char *)write_buf), &count);

        if(res != FR_OK)
        {
            printf("f_write 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi_SD文件\r\n");
            count = 0;
            f_close(&SDFile);
        }
        else
        {
            printf("文件写入成功，写入字节数据：%d\n", count);
            printf("向文件写入的数据为：\r\n%s\r\n", write_buf);
            printf("关闭打开的BearPi_SD文件\r\n");
            count = 0;
            f_close(&SDFile);
        }
    }
    else printf("打开/创建BearPi.txt文件失败，err = %d\r\n", res);

    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res = f_open(&SDFile, "0:BearPi_SD.txt", FA_OPEN_EXISTING | FA_READ);

    if(res == FR_OK)
    {
        printf("打开BearPi_SD.txt文件成功\r\n");
        res = f_read(&SDFile, read_buf, sizeof(read_buf), &count);

        if(res != FR_OK)
        {
            printf("f_read 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi_SD.txt文件\r\n");
            f_close(&SDFile);
        }
        else
        {
            printf("文件读取成功，读取字节数据：%d\n", count);
            printf("向文件读取的数据为：\r\n%s\r\n", read_buf);
            printf("关闭打开的BearPi_SD.txt文件\r\n");
            f_close(&SDFile);
        }
    }
    else printf("打开BearPi_SD.txt文件失败，err = %d\r\n", res);

    /*------------------- 不再使用文件系统，取消挂载文件系统 ------------------------------------*/
    printf("不再使用文件系统，取消挂载文件系统\r\n");
    res = f_mount(NULL, "0:", 1);

    if(res == FR_OK) printf("取消挂载文件系统成功\r\n");
    else    printf("取消挂载文件系统失败，err = %d\r\n", res);

    printf("SD卡fatfs文件系统测试结束\r\n");
}

//测试spi_flash挂载的Fatfs文件系统
void test_spi_flash_fatfs(void)
{
    uint8_t res ;
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    printf("\r\n****** 即将进行QSPI Fatfs文件写入测试... ******\r\n");
    res = f_open(&USERFile, "1:BearPi.txt", FA_OPEN_ALWAYS | FA_WRITE);

    if(res == FR_OK)
    {
        printf("打开/创建BearPi.txt文件成功，向文件写入数据。\r\n");
        res = f_write(&USERFile, write_buf, strlen((const char *)write_buf), &count);

        if(res != FR_OK)
        {
            printf("f_write 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi.txt文件\r\n");
            count = 0;
            f_close(&USERFile);
        }
        else
        {
            printf("文件写入成功，写入字节数据：%d\n", count);
            printf("向文件写入的数据为：\r\n%s\r\n", write_buf);
            printf("关闭打开的BearPi.txt文件\r\n");
            count = 0;
            f_close(&USERFile);
        }
    }
    else printf("打开/创建BearPi.txt文件失败，err = %d\r\n", res);

    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res = f_open(&USERFile, "1:BearPi.txt", FA_OPEN_EXISTING | FA_READ);

    if(res == FR_OK)
    {
        printf("打开BearPi.txt文件成功\r\n");
        res = f_read(&USERFile, read_buf, sizeof(read_buf), &count);

        if(res != FR_OK)
        {
            printf("f_read 发生错误，err = %d\r\n", res);
            printf("关闭打开的BearPi.txt文件\r\n");
            f_close(&USERFile);
        }
        else
        {
            printf("文件读取成功，读取字节数据：%d\n", count);
            printf("向文件读取的数据为：\r\n%s\r\n", read_buf);
            printf("关闭打开的BearPi.txt文件\r\n");
            f_close(&USERFile);
        }
    }
    else printf("打开BearPi.txt文件失败，err = %d\r\n", res);

    /*------------------- 不再使用文件系统，取消挂载文件系统 ------------------------------------*/
    printf("不再使用文件系统，取消挂载文件系统\r\n");
    res = f_mount(NULL, "1:", 1);

    if(res == FR_OK) printf("取消挂载文件系统成功\r\n");
    else    printf("取消挂载文件系统失败，err = %d\r\n", res);

    printf("QSPI fatfs文件系统测试结束\r\n");
}
