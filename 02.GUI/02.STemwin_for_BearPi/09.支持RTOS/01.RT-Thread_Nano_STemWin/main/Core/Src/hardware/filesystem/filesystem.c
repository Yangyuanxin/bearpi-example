#include "filesystem.h"
#include "fatfs.h"
#include "rtc.h"


/*����SD��*/
int Mount_SD(void)
{
    /*����SD��*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    return 0 ;
}


