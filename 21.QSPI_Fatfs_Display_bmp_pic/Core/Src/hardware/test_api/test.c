#include "test.h"
#include "fatfs.h"

uint32_t count = 0;
uint8_t read_buf[50] = {0};
/*�����ļ�ϵͳ*/
uint8_t work[_MAX_SS] = {0} ;
uint8_t write_buf[50] = "hello BearPi\r\n";


/*����SD��*/
int Mount_SD_Fatfs(void)
{
    /*����SD��*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    printf("SD���ļ�ϵͳ���سɹ�!\n");
    return 0 ;
}

/*����QSPI_FLASH��fatfs�ļ�ϵͳ*/
void Mount_QSPI_FLASH_Fatfs(void)
{
    retUSER = f_mount(&USERFatFS, USERPath, 1);

    if(retUSER != FR_OK)
    {
        if(retUSER == FR_NO_FILESYSTEM)
        {
            printf("f_mount û���ļ�ϵͳ,��ʼ��ʽ��spi-flash\r\n");
            retUSER = f_mkfs(USERPath, FM_ANY, 0, work, sizeof(work));

            if(retUSER != FR_OK)
            {
                printf("f_mkfs ��ʽ��ʧ�ܣ�err = %d\r\n", retUSER);
            }
            else
            {
                printf("��ʽ���ɹ�����ʼ���¹���spi-flash\r\n");
                retUSER = f_mount(&USERFatFS, USERPath, 1);

                if(retUSER != FR_OK)
                {
                    printf("f_mount ��������err = %d\r\n", retUSER);
                }
                else
                {
                    printf("spi-flash�ļ�ϵͳ���سɹ�\r\n");
                }
            }
        }
        else
        {
            printf("f_mount ������������err = %d\r\n", retUSER);
        }
    }
    else
        printf("qspi�ļ�ϵͳ���سɹ�\r\n");
}

/*��ȡ���ڴ�*/
uint8_t  f_GetTotal_Free(uint8_t *drv, uint32_t *total, uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);//�õ�������Ϣ�����д�����

    if(res == 0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize; //�õ���������
        fre_sect = fre_clust * fs1->csize;        //�õ�����������
        #if _MAX_SS!=512                                  //������С����512�ֽ�,��ת��Ϊ512�ֽ�
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
        #endif
        *total = tot_sect >> 1; //��λΪKB
        *free = fre_sect >> 1; //��λΪKB
    }

    return res;
}

void test_get_sd_card_cap(void)
{
    uint32_t Total = 0; //��ȡSD��������
    uint32_t Free = 0; //��ȡSD��ʣ������
    f_GetTotal_Free((uint8_t*)"0:", &Total, &Free); //��ȡSD����������ʣ������
    printf("��ǰFatfs������:%dKB==>%dMB ʣ������:%dKB==>%dMB\n", Total, Total / 1024, Free, Free / 1024);
}

void test_get_qspi_cap(void)
{
    uint32_t Total = 0; //��ȡFLASH������
    uint32_t Free = 0; //��ȡFLASHʣ������
    f_GetTotal_Free((uint8_t*)"1:", &Total, &Free); //��ȡFatfs��������ʣ������
    printf("��ǰFatfs������:%dKB==>%dMB ʣ������:%dKB==>%dMB\n", Total, Total / 1024, Free, Free / 1024);
}


//����SD�����ص�Fatfs�ļ�ϵͳ
void test_sd_card_fatfs(void)
{
    uint8_t res ;
    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    printf("\r\n****** ��������SD��Fatfs�ļ�д�����... ******\r\n");
    res = f_open(&SDFile, "0:BearPi_SD.txt", FA_OPEN_ALWAYS | FA_WRITE);

    if(res == FR_OK)
    {
        printf("��/����BearPi_SD�ļ��ɹ������ļ�д�����ݡ�\r\n");
        res = f_write(&SDFile, write_buf, strlen((const char *)write_buf), &count);

        if(res != FR_OK)
        {
            printf("f_write ��������err = %d\r\n", res);
            printf("�رմ򿪵�BearPi_SD�ļ�\r\n");
            count = 0;
            f_close(&SDFile);
        }
        else
        {
            printf("�ļ�д��ɹ���д���ֽ����ݣ�%d\n", count);
            printf("���ļ�д�������Ϊ��\r\n%s\r\n", write_buf);
            printf("�رմ򿪵�BearPi_SD�ļ�\r\n");
            count = 0;
            f_close(&SDFile);
        }
    }
    else printf("��/����BearPi.txt�ļ�ʧ�ܣ�err = %d\r\n", res);

    /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    printf("****** ���������ļ���ȡ����... ******\r\n");
    res = f_open(&SDFile, "0:BearPi_SD.txt", FA_OPEN_EXISTING | FA_READ);

    if(res == FR_OK)
    {
        printf("��BearPi_SD.txt�ļ��ɹ�\r\n");
        res = f_read(&SDFile, read_buf, sizeof(read_buf), &count);

        if(res != FR_OK)
        {
            printf("f_read ��������err = %d\r\n", res);
            printf("�رմ򿪵�BearPi_SD.txt�ļ�\r\n");
            f_close(&SDFile);
        }
        else
        {
            printf("�ļ���ȡ�ɹ�����ȡ�ֽ����ݣ�%d\n", count);
            printf("���ļ���ȡ������Ϊ��\r\n%s\r\n", read_buf);
            printf("�رմ򿪵�BearPi_SD.txt�ļ�\r\n");
            f_close(&SDFile);
        }
    }
    else printf("��BearPi_SD.txt�ļ�ʧ�ܣ�err = %d\r\n", res);

    /*------------------- ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ ------------------------------------*/
    printf("����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ\r\n");
    res = f_mount(NULL, "0:", 1);

    if(res == FR_OK) printf("ȡ�������ļ�ϵͳ�ɹ�\r\n");
    else    printf("ȡ�������ļ�ϵͳʧ�ܣ�err = %d\r\n", res);

    printf("SD��fatfs�ļ�ϵͳ���Խ���\r\n");
}

//����spi_flash���ص�Fatfs�ļ�ϵͳ
void test_spi_flash_fatfs(void)
{
    uint8_t res ;
    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    printf("\r\n****** ��������QSPI Fatfs�ļ�д�����... ******\r\n");
    res = f_open(&USERFile, "1:BearPi.txt", FA_OPEN_ALWAYS | FA_WRITE);

    if(res == FR_OK)
    {
        printf("��/����BearPi.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
        res = f_write(&USERFile, write_buf, strlen((const char *)write_buf), &count);

        if(res != FR_OK)
        {
            printf("f_write ��������err = %d\r\n", res);
            printf("�رմ򿪵�BearPi.txt�ļ�\r\n");
            count = 0;
            f_close(&USERFile);
        }
        else
        {
            printf("�ļ�д��ɹ���д���ֽ����ݣ�%d\n", count);
            printf("���ļ�д�������Ϊ��\r\n%s\r\n", write_buf);
            printf("�رմ򿪵�BearPi.txt�ļ�\r\n");
            count = 0;
            f_close(&USERFile);
        }
    }
    else printf("��/����BearPi.txt�ļ�ʧ�ܣ�err = %d\r\n", res);

    /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    printf("****** ���������ļ���ȡ����... ******\r\n");
    res = f_open(&USERFile, "1:BearPi.txt", FA_OPEN_EXISTING | FA_READ);

    if(res == FR_OK)
    {
        printf("��BearPi.txt�ļ��ɹ�\r\n");
        res = f_read(&USERFile, read_buf, sizeof(read_buf), &count);

        if(res != FR_OK)
        {
            printf("f_read ��������err = %d\r\n", res);
            printf("�رմ򿪵�BearPi.txt�ļ�\r\n");
            f_close(&USERFile);
        }
        else
        {
            printf("�ļ���ȡ�ɹ�����ȡ�ֽ����ݣ�%d\n", count);
            printf("���ļ���ȡ������Ϊ��\r\n%s\r\n", read_buf);
            printf("�رմ򿪵�BearPi.txt�ļ�\r\n");
            f_close(&USERFile);
        }
    }
    else printf("��BearPi.txt�ļ�ʧ�ܣ�err = %d\r\n", res);

    /*------------------- ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ ------------------------------------*/
    printf("����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ\r\n");
    res = f_mount(NULL, "1:", 1);

    if(res == FR_OK) printf("ȡ�������ļ�ϵͳ�ɹ�\r\n");
    else    printf("ȡ�������ļ�ϵͳʧ�ܣ�err = %d\r\n", res);

    printf("QSPI fatfs�ļ�ϵͳ���Խ���\r\n");
}
