#include "filesystem.h"
#include "fatfs.h"
#include "rtc.h"

uint8_t work[_MAX_SS] ;
Record_Data csv_file_record ;

/*�����ļ�ϵͳ*/
void Mount_Fatfs(void)
{
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(retSD != FR_OK)
    {
        if(retSD == FR_NO_FILESYSTEM)
        {
            DEBUG("f_mount û���ļ�ϵͳ,��ʼ��ʽ��SD��\r\n");
            retSD = f_mkfs(SDPath, FM_ANY, 0, work, sizeof(work));

            if(retSD != FR_OK)
            {
                DEBUG("f_mkfs ��ʽ��ʧ�ܣ�err = %d\r\n", retSD);
            }
            else
            {
                DEBUG("��ʽ���ɹ�����ʼ���¹���spi-flash\r\n");
                retSD = f_mount(&SDFatFS, SDPath, 1);

                if(retSD != FR_OK)
                {
                    DEBUG("f_mount ��������err = %d\r\n", retSD);
                }
                else
                {
                    DEBUG("SD���ļ�ϵͳ���سɹ�\r\n");
                }
            }
        }
        else
        {
            DEBUG("f_mount ������������err = %d\r\n", retSD);
        }
    }
    else
        DEBUG("SD���ļ�ϵͳ���سɹ�\r\n");
}

/*����ļ�ϵͳ����*/
void UMount_Fatfs(void)
{
    DEBUG("����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ\r\n");
    retSD = f_mount(NULL, SDPath, 1);

    if(retSD == FR_OK)
        DEBUG("ȡ�������ļ�ϵͳ�ɹ�\r\n");
    else
        DEBUG("ȡ�������ļ�ϵͳʧ�ܣ�err = %d\r\n", retSD);
}


/*��ȡ�ļ����һ������*/
int ReadlLineData(int File_Count, int line_number)
{
	/*ָ��Ҫ��ȡ��һ��*/
    int WhichLine ;     
	/*��ǰ��ȡ����*/
    int CurrentIndex = 0;        
    char *StrLine = NULL ;
    char *ptr = NULL ;
    int data_serial_num_file_count ;
    char filename[FILE_NAME_LEN] = {0};
    WhichLine = line_number - 1;
    StrLine = malloc(100) ;
    assert_param(NULL != strLine);
    /*���㵱ǰ����ˮ������Ӧ���ļ�*/
    data_serial_num_file_count = File_Count ;
    sprintf(filename, "0:BearPi_Log%d.csv", data_serial_num_file_count);
    retSD = f_open(&SDFile, filename, FA_OPEN_EXISTING | FA_READ);
    if(FR_OK != retSD)
    {
        DEBUG("��%s�ļ�ʧ�ܣ�err = %d\r\n", filename, retSD);
        free(StrLine);
        return -2;
    }
    while(!f_eof(&SDFile))
    {
        if(CurrentIndex == WhichLine)
        {
            //��ȡһ��
            f_gets(StrLine, 100, &SDFile);
            DEBUG("��ǰ����:%s\n", StrLine);
            //Para1:��ȡ��ˮ��
            csv_file_record.serial_number = atoi(StrLine);
            //Para2:��ȡ��
            ptr = strstr(StrLine, ",");
            csv_file_record.year = atoi(ptr + 1);
            //Para3:��ȡ��
            ptr = strstr(ptr + 1, "/");
            csv_file_record.month = atoi(ptr + 1);
            //Para4:��ȡ��
            ptr = strstr(ptr + 1, "/");
            csv_file_record.day = atoi(ptr + 1);
            //Para5:��ȡʱ
            ptr = strstr(ptr + 1, " ");
            csv_file_record.hour = atoi(ptr + 1);
            //Para6:��ȡ��
            ptr = strstr(ptr + 1, ":");
            csv_file_record.minute = atoi(ptr + 1);
            //Para7:��ȡ�����===>Ϊ0���ǰ�ȫ��Ϊ1����Σ��
            ptr = strstr(ptr + 1, ",");
            csv_file_record.detect_result = atoi(ptr + 1);
        }
        //��ȡһ��,����λ����һ��
        f_gets(StrLine, 100, &SDFile);
        CurrentIndex++;
    }
    f_close(&SDFile);
    free(StrLine);
    return CurrentIndex ;
}


/*���������ݵ�csv�ļ���*/
int save_record_to_flash(void)
{
    uint32_t count = 0  ;
    char filename[FILE_NAME_LEN] = {0};
    char Detect_Data[100] = {0} ;
    /*���㵱ǰ��ˮ��һ�����Բ������ٸ��ļ���һ���ļ�����100����¼�����Զ������µ��ļ�*/
    int data_serial_num_file_count =  User_Memory_Para.detetct_log_serial_number / DETECT_DATA_INDEX ;
    sprintf(filename, "0:BearPi_Log%d.csv", data_serial_num_file_count);
    retSD = f_open(&SDFile, filename, FA_OPEN_ALWAYS | FA_WRITE);

    if(FR_OK != retSD)
    {
        DEBUG("��/����%s�ļ�ʧ�ܣ�err = %d\r\n", filename, retSD);
        return -1 ;
    }
    printf("��/����%s�ļ��ɹ������ļ�д�����ݡ�\r\n", filename);
    /*ÿһ�ζ�ƫ�Ƶ��ļ���ĩβ*/
    f_lseek(&SDFile, f_size(&SDFile));
    /*��ˮ������*/
    ++User_Memory_Para.detetct_log_serial_number;
    /*��¼��ˮ�ű��洦��*/
    User_Detect_Log_Save_Process();
    sprintf(Detect_Data, "%d,%2d/%02d/%02d %02d:%02d,%d\r\n",
            User_Memory_Para.detetct_log_serial_number, DateTime_Handler_Info.year, 
			DateTime_Handler_Info.month,DateTime_Handler_Info.day, 
			DateTime_Handler_Info.hour, DateTime_Handler_Info.minute, Sensor_Flow_Cursor.Is_safety_or_danger);
	retSD = f_write(&SDFile, Detect_Data, strlen((char *)Detect_Data), &count);
	if(retSD != FR_OK)
    {
        DEBUG("f_write ��������err = %d\r\n", retSD);
        DEBUG("�رմ򿪵�BearPi_Log.csv�ļ�\r\n");
        count = 0;
        memset(Detect_Data, 0, DETECT_DATA_LEN);
        f_close(&SDFile);
        return -2 ;
    }
	DEBUG("�ļ�д��ɹ���д���ֽ����ݣ�%d\n", count);
    DEBUG("���ļ�д�������Ϊ��\r\n%s\r\n", Detect_Data);
    DEBUG("�رմ򿪵�BearPi_Log.csv�ļ�\r\n");
    count = 0;
    memset(Detect_Data, 0, DETECT_DATA_LEN);
    f_close(&SDFile);
	return 0 ;
}



