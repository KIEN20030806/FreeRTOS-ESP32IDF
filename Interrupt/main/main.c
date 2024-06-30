#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define INTERRUPT_PIN 4

static const char *TAG = "example";
SemaphoreHandle_t interruptSemaphore;

void IRAM_ATTR interruptHandler(void* arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(interruptSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

void TaskLedon(void *pvParameters)
{
    while (1) {
        if (xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS) {
            printf("TaskLedon Received Semaphore\n");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void TaskLedoff(void *pvParameters)
{
    while (1) {
        if (xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS) {
            printf("TaskLedoff Received Semaphore\n");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void app_main()
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    
    interruptSemaphore = xSemaphoreCreateBinary();
    if (interruptSemaphore != NULL) {
        esp_rom_gpio_pad_select_gpio(INTERRUPT_PIN);
        gpio_set_direction(INTERRUPT_PIN, GPIO_MODE_INPUT);
        gpio_set_intr_type(INTERRUPT_PIN, GPIO_INTR_HIGH_LEVEL);
        
        xTaskCreate(TaskLedon, "TaskLedon", 4096, NULL, 1, NULL);
        xTaskCreate(TaskLedoff, "TaskLedoff", 4096, NULL, 1, NULL);
        
        gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
        gpio_isr_handler_add(INTERRUPT_PIN, interruptHandler, (void*) INTERRUPT_PIN);
    } else {
        printf("Failed to create semaphore\n");
    }
}
