#include <stdio.h>
#include "time.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define idleTaskDuration 5000
#define preemptTaskDuration 1500
#define delay 1000

void vIdleTask(void *pv)
{
    clock_t begin;
    int runTime;
    for (;;)
    {
        begin = clock();
        runTime = 0;
        printf("%s begins at %f\n", (char *)pv, (double)((double)begin / CLOCKS_PER_SEC));
        while (runTime < idleTaskDuration)
            runTime += 1;
        printf("%s ends at %f from %f\n", (char *)pv, (double)((double)clock() / CLOCKS_PER_SEC), (double)((double)begin / CLOCKS_PER_SEC));
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void vPreemptTask(void *pv)
{
    clock_t begin;
    int runTime;
    for (;;)
    {
        begin = clock();
        runTime = 0;
        printf("%s begins at %f\n", (char *)pv, (double)((double)begin / CLOCKS_PER_SEC));
        while (runTime < idleTaskDuration)
            runTime += 1;
        printf("%s ends at %f from %f\n", (char *)pv, (double)((double)clock() / CLOCKS_PER_SEC), (double)((double)begin / CLOCKS_PER_SEC));
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

static const char *pcTextForTask1 = " Idle task 1\r\n";
static const char *pcTextForTask2 = " Idle task 2\r\n";
static const char *pcTextForTask3 = " Task 3\r\n";

void app_main(void)
{
    printf("configUSE_PREEMPTION: %d\n", configUSE_PREEMPTION);
    printf("configUSE_TIME_SLICING: %d\n", configUSE_TIME_SLICING);
    xTaskCreatePinnedToCore(&vIdleTask, "Task 1", 4096, (void *)pcTextForTask1, 1, NULL, 1);
    vTaskDelay(200 / portTICK_RATE_MS);
    xTaskCreatePinnedToCore(&vIdleTask, "Task 2", 4096, (void *)pcTextForTask2, 1, NULL, 1);
    vTaskDelay(1200 / portTICK_RATE_MS);
    xTaskCreatePinnedToCore(&vPreemptTask, "Task 3", 4096, (void *)pcTextForTask3, 2, NULL, 1);
}
