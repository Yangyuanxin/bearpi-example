#ifndef __TEST_H
#define __TEST_H
#include "main.h"






/*����SD��*/
int Mount_SD_Fatfs(void);
/*����QSPI_FLASH��fatfs�ļ�ϵͳ*/
void Mount_QSPI_FLASH_Fatfs(void);
/*��ȡ���ڴ�*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free);

//����SD�����ص�Fatfs�ļ�ϵͳ
void test_sd_card_fatfs(void);
//����spi_flash���ص�Fatfs�ļ�ϵͳ
void test_spi_flash_fatfs(void);
//����SD������
void test_get_sd_card_cap(void);
//����QSPI����
void test_get_qspi_cap(void);

#endif //__TEST_H

