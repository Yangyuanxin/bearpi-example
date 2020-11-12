#include "para.h"
#include "iniparser.h"

dictionary  *Config_ini = NULL;
Conf_Para User_Memory_Para ;

/*默认系统配置信息*/
char *System_Config_Info =
{
    "[Setting]\n"
    "Vol_flag=1;\n"					/*蜂鸣器开关*/
    "Sensivity=1;\n"				/*灵敏度级别  0:低 1:中 2:高*/
    "Debug_flag=1;\n"				/*调试标志*/
    "Detect_Log_Serial_Number=0;\n"	/*检测数据流水号*/
    "SENSIVITY_LOW    = 1000;\n"    /*低灵敏度*/
    "SENSIVITY_MEDIUM = 300;\n"     /*中灵敏度*/
    "SENSIVITY_HIGH   = 200;\n"     /*高灵敏度*/
};

/*创建一个默认的配置文件*/
static void Create_Default_InI_File(void)
{
    retUSER_SYS_CONFIG = f_open(&USER_SYS_CONFIG_File, SETTING_PARA, FA_OPEN_ALWAYS | FA_WRITE);

    if(FR_OK != retUSER_SYS_CONFIG)
    {
        fprintf(stderr, "iniparser: cannot open %s\n", SETTING_PARA);
        return ;
    }

    f_printf(&USER_SYS_CONFIG_File, System_Config_Info);
    f_close(&USER_SYS_CONFIG_File);
}

/*加载系统参数*/
static void Load_System_Para(void)
{
    /*音量*/
    User_Memory_Para.volume = iniparser_getint(Config_ini, "Setting:Vol_flag", -1);

    if(-1 == User_Memory_Para.volume)
        User_Memory_Para.volume = 1 ;

    /*灵敏度*/
    User_Memory_Para.sensivity = iniparser_getint(Config_ini, "Setting:Sensivity", -1);

    if(-1 == User_Memory_Para.sensivity)
        User_Memory_Para.sensivity = 1 ;

    /*调试标志*/
    User_Memory_Para.debug_flag = iniparser_getint(Config_ini, "Setting:Debug_flag", -1);

    if(-1 == User_Memory_Para.debug_flag)
        User_Memory_Para.debug_flag = 1 ;

    DEBUG("音量:%d\n", User_Memory_Para.volume);
    DEBUG("灵敏度:%d\n", User_Memory_Para.sensivity);
    DEBUG("调试标志:%d\n", User_Memory_Para.debug_flag);

    /*检测记录流水号*/
    User_Memory_Para.detetct_log_serial_number = iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1);

    if(-1 == User_Memory_Para.detetct_log_serial_number)
        User_Memory_Para.detetct_log_serial_number = 0 ;

    DEBUG("检测流水号:%d\n", User_Memory_Para.detetct_log_serial_number);
    User_Memory_Para.alarm_threshold[0] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_LOW", -1);
    User_Memory_Para.alarm_threshold[1] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_MEDIUM", -1);
    User_Memory_Para.alarm_threshold[2] = \
                                          iniparser_getint(Config_ini, "Setting:SENSIVITY_HIGH", -1);

    for(int i = 0 ; i < 3 ; i++)
    {
        if(-1 == User_Memory_Para.alarm_threshold[i])
        {
            User_Memory_Para.alarm_threshold[0] = 1000 ;
            User_Memory_Para.alarm_threshold[1] = 300 ;
            User_Memory_Para.alarm_threshold[2] = 200 ;
            break ;
        }
    }

    for(int i = 0 ; i < 3 ; i++)
        DEBUG("灵敏度:%d  阈值:%d\n", i, User_Memory_Para.alarm_threshold[i]);
}

/*加载配置文件*/
void Load_Config_ini_File(void)
{
    Config_ini = iniparser_load(SETTING_PARA);

    /*如果打开失败了，则创建一个全新的ini配置文件，然后打开这个文件进行后续的解析工作*/
    if(NULL == Config_ini)
    {
        DEBUG("cannot parse %s file\n", SETTING_PARA);
        Create_Default_InI_File();
        Config_ini = iniparser_load(SETTING_PARA);

        if(NULL == Config_ini)
        {
            DEBUG("创建默认INI文件后继续加载出错\n");
            return  ;
        }
    }

    DEBUG("加载INI文件成功\n");
    /*加载系统参数*/
    Load_System_Para();
}

/*设置调试*/
void setting_debug(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.debug_flag = status ;
    iniparser_set(Config_ini, "Setting:Debug_flag", buf);
    DEBUG("设置调试:%d\r\n", iniparser_getint(Config_ini, "Setting:Debug_flag", -1));
    User_Para_Save_Process();
}

/*设置音量*/
void setting_volume(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.volume = status ;
    iniparser_set(Config_ini, "Setting:Vol_flag", buf);
    DEBUG("设置音量:%d\r\n", iniparser_getint(Config_ini, "Setting:Vol_flag", -1));
    User_Para_Save_Process();
}

/*设置灵敏度*/
void setting_sensivity(uint8_t status)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", status);
    User_Memory_Para.sensivity = status ;
    iniparser_set(Config_ini, "Setting:Sensivity", buf);
    DEBUG("设置灵敏度:%d\r\n", iniparser_getint(Config_ini, "Setting:Sensivity", -1));
    User_Para_Save_Process();
}


/*设置阈值*/
void Alarm_Threhold_Setting(void)
{
    char buf1[5] = {0};
    char buf2[5] = {0};
    char buf3[5] = {0};
    sprintf(buf1, "%d", User_Memory_Para.alarm_threshold[0]);
    sprintf(buf2, "%d", User_Memory_Para.alarm_threshold[1]);
    sprintf(buf3, "%d", User_Memory_Para.alarm_threshold[2]);

    iniparser_set(Config_ini, "Setting:SENSIVITY_LOW", buf1);		/*低灵敏度*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_MEDIUM", buf2);	/*中灵敏度*/
    iniparser_set(Config_ini, "Setting:SENSIVITY_HIGH", buf3);		/*高灵敏度*/
    User_Para_Save_Process();
}

/*记录流水号保存处理*/
void User_Detect_Log_Save_Process(void)
{
    char buf[5] = {0};
    memset(buf, 0, 5);
    sprintf(buf, "%d", User_Memory_Para.detetct_log_serial_number);
    iniparser_set(Config_ini, "Setting:Detect_Log_Serial_Number", buf);
    DEBUG("设置检测流水号:%d\r\n", iniparser_getint(Config_ini, "Setting:Detect_Log_Serial_Number", -1));
    User_Para_Save_Process();
}

/*用户参数保存处理*/
void User_Para_Save_Process(void)
{
    //锁定系统调度
    tos_knl_sched_lock();
    /*write config.ini parse*/
    retUSER_SYS_CONFIG = f_open(&USER_SYS_CONFIG_File, SETTING_PARA, FA_OPEN_EXISTING | FA_WRITE);

    if(FR_OK != retUSER_SYS_CONFIG)
    {
        printf("iniparser: cannot open %s\n", SETTING_PARA);
        return ;
    }

    printf("参数设置保存成功\n");
    iniparser_dump_ini(Config_ini, &USER_SYS_CONFIG_File);
    f_close(&USER_SYS_CONFIG_File);
    iniparser_freedict(Config_ini);
    Load_Config_ini_File();
    //解锁系统调度
    tos_knl_sched_unlock();
}

