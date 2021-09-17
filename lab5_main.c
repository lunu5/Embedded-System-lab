#include <stdio.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <freertos/timers.h>

#define xTicksToWait 0
#define PRINTTIME_1 10;
#define PRINTTIME_2 5;
#define PERIOD_1 200;
#define PERIOD_2 300;

xTimerHandle timer1 = NULL;
xTimerHandle timer2 = NULL;
static int count1 = 0;
static int count2 = 0;

void task(xTimerHandle pxTimer)
{
    int timerId = (int)pvTimerGetTimerID(pxTimer);
    if (timerId == 1)
    {
        if (count1 < PRINTTIME_1)
        {
            printf("ahihi time %d at %d ms\n", count1, (int)clock());
            count1++;
            if (count1 == PRINTTIME_1)
                xTimerStop(timer1, xTicksToWait);
        }
    }
    else
    {
        if (count2 < PRINTTIME_2)
        {
            printf("ihaha time %d at %d ms\n", count2, (int)clock());
            count2++;
            if (count1 == PRINTTIME_2)
                xTimerStop(timer2, xTicksToWait);
        }
    }
}
void app_main(void)
{
    timer1 = xTimerCreate("ahihi", PERIOD_1, pdTrue, (void *)1, task);
    timer2 = xTimerCreate("ihaha", PERIOD_2, pdTrue, (void *)2, task);
    xTimerStart(timer1, xTicksToWait);
    xTimerStart(timer2, xTicksToWait);
}
