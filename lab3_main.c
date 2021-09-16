#include <stdio.h>
#include "time.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#define duration 1000

void task(void *pv)
{
    for (;;)
    {
        clock_t begin = clock();
        printf("%s begins at %f\n", (char *)pv, (double)((double)begin / CLOCKS_PER_SEC));
        printf("configUSE_PREEMPTION: %d\n", configUSE_PREEMPTION);
        printf("configUSE_TIME_SLICING: %d\n", configUSE_TIME_SLICING);
        while ((double)(clock() - begin) < duration){}
        printf("%s ends at %f from %f\n", (char *)pv, (double)((double)clock() / CLOCKS_PER_SEC), (double)((double)begin / CLOCKS_PER_SEC));
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}
static const char *pcTextForTask1 = " Task 1\r\n";
static const char *pcTextForTask2 = " Task 2\r\n";
static const char *pcTextForTask3 = " Task 3\r\n";

void app_main(void)
{
    xTaskCreatePinnedToCore(&task, "Task 1", 10000, (void *)pcTextForTask1, 1, NULL, 1);
    vTaskDelay(400 / portTICK_RATE_MS);
    xTaskCreatePinnedToCore(&task, "Task 2", 10000, (void *)pcTextForTask2, 1, NULL, 1);
    vTaskDelay(4200 / portTICK_RATE_MS);
    xTaskCreatePinnedToCore(&task, "Task 3", 5000, (void *)pcTextForTask3, 2, NULL, 1);
}
