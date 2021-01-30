#include "filesystem.h"
#include "fatfs.h"
#include "rtc.h"


/*π“‘ÿSDø®*/
int Mount_SD(void)
{
    /*π“‘ÿSDø®*/
    retSD = f_mount(&SDFatFS, SDPath, 1);

    if(FR_OK != retSD)
        return -1 ;

    return 0 ;
}


