#ifndef __SENSOR_H
#define __SENSOR_H
#include "main.h"

typedef struct Sensor
{
    /*烟感值*/
    int Smoke_Value;
    /*报警灯状态*/
    uint8_t Alarm_Led_Status : 1;
    /*报警蜂鸣器状态*/
    uint8_t Alarm_Buzzer_Status : 1;

    void (*led_control)(struct Sensor *sensor_handle, uint8_t status);
    void (*buzzer_control)(struct Sensor *sensor_handle, uint8_t status);

    int (*get_smoke_value)(struct Sensor *sensor_handle);
    int (*get_led_status)(struct Sensor *sensor_handle);
    int (*get_buzzer_status)(struct Sensor *sensor_handle);
} Sensor ;
/*提供访问传感器的接口*/
extern Sensor mq2_sensor_interface;
/*注册传感器*/
int Sensor_Register(Sensor *sensor_handle);


#endif //__SENSOR_H

