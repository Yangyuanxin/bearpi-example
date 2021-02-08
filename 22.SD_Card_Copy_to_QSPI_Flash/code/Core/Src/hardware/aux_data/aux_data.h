#ifndef __AUX_DATA_H
#define	__AUX_DATA_H

#include "main.h"
#include "ff.h"
#include "string.h"
//SD卡及flash的根目录
#define SD_ROOT       "0:"
#define FLASH_ROOT    "1:"

int copy_file(char *src_path, char *dst_path);
FRESULT scan_files (char* src_path);
/*从SD卡拷贝UI文件到QSPI FLASH*/
FRESULT copy_ui_files (char* src_path);

#endif /* __BURN_DATA_H */
