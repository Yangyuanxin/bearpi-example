#ifndef __TEST_H
#define __TEST_H
#include "main.h"






/*挂载SD卡*/
int Mount_SD_Fatfs(void);
/*挂载QSPI_FLASH的fatfs文件系统*/
void Mount_QSPI_FLASH_Fatfs(void);
/*获取盘内存*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free);

//测试SD卡挂载的Fatfs文件系统
void test_sd_card_fatfs(void);
//测试spi_flash挂载的Fatfs文件系统
void test_spi_flash_fatfs(void);
//测试SD卡容量
void test_get_sd_card_cap(void);
//测试QSPI容量
void test_get_qspi_cap(void);

#endif //__TEST_H

