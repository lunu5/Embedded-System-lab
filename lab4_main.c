#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define functionalTaskNumber 3
#define delay 1000
#define xTicksToWait 100

typedef struct Data_t
{
    int taskId;
    int classify;
    int functionRecieved;
} Data_t;

static QueueHandle_t queue;

void vReceptionTask(void *pv)
{
    for (int i = 0;; i++)
    {
        Data_t task;
        task.eDataID = i;
        if (i % 2 == 0)
            task.classify = 0;
        else if (i % 3 == 0)
            task.classify = 1;
        else if (i % 7 == 0)
            task.classify = 2;
        else
            task.classify = -1;
        task.functionRecieved = 0;

        xQueueSend(queue, &task, xTicksToWait);
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void vFunctionalTask(void *pv)
{
    Data_t task;
    for (;;)
    {
        if (xQueueReceive(queue, (void *)&task, xTicksToWait) == pdTRUE)
        {
            if (((task.classify == 0) && ((char *)pv == "Functional task 1")) || ((task.classify == 1) && ((char *)pv == "Functional task 2")) || ((task.classify == 2) && ((char *)pv == "Functional task 3")))
                printf("%s received and is executing request id %d", (char *)pv, task.taskId);
            else
            {
                task.functionRecieved += 1;
                if (functionalTaskNumber > task.functionRecieved)
                    xQueueSendToFront(queue, &task, xTicksToWait);
                else
                    printf("Error: no functional task execute request id %d", task.taskId);
            }
        }
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

static const char *pcTextForTask0 = "Reception task";
static const char *pcTextForTask1 = "Functional task 1";
static const char *pcTextForTask2 = "Functional task 2";
static const char *pcTextForTask3 = "Functional task 3";

void app_main(void)
{
    queue = xQueueCreate(10, sizeof(Data_t));
    xTaskCreatePinnedToCore(&vReceptionTask, "Reception task", 4096, (void *)pcTextForTask0, 2, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 1", 4096, (void *)pcTextForTask1, 1, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 2", 4096, (void *)pcTextForTask2, 1, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 3", 4096, (void *)pcTextForTask3, 1, NULL, 1);
}
