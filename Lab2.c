#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_spi_flash.h"

#define LED_GPIO GPIO_NUM_15
#define BTN_GPIO GPIO_NUM_17

void AFunc() {
    int iVariableExample = 0;
    for (;;) {
        printf("Luu Nguyen - 1852622 \n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void print_ESP32(void* pvParameters) 
{
    /* Configure the IOMUX register for pad LED_GPIO, BTN_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(LED_GPIO);
    gpio_pad_select_gpio(BTN_GPIO);

    /* Set the GPIO as a push/pull output */
    gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BTN_GPIO);
	gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
    while (1) {
        if (gpio_get_level(BTN_GPIO)==1){
	            printf("ESP32\n");
                gpio_set_level(LED_GPIO, 1);      
        }
        else gpio_set_level(LED_GPIO, 0);
        vTaskDelay(100 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(&print_ESP32, "button_task", 1024, NULL, 0, NULL);
    xTaskCreate(&AFunc, "Cycle", 1024, NULL, 0, NULL);
}
