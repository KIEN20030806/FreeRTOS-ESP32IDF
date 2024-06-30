///////////////mutex////////
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

SemaphoreHandle_t xMutex;

void lowPriorityTask(void *parameter) {
    // printf("lowPriorityTask", "%s", (char *)parameter);
    for (;;) {
        printf( "lowPriorityTask gains key\n");
        xSemaphoreTake(xMutex, portMAX_DELAY);
        /* even low priority task delay high priority 
        still in Block state */
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("lowPriorityTask releases key\n");
        xSemaphoreGive(xMutex);
    }
    vTaskDelete(NULL);
}

void highPriorityTask(void *parameter) {
    // printf("highPriorityTask", "%s", (char *)parameter);
    for (;;) {
        printf( "highPriorityTask gains key\n");
        /* highPriorityTask wait until lowPriorityTask release key */
        xSemaphoreTake(xMutex, portMAX_DELAY);
        printf( "highPriorityTask is running\n");
        printf( "highPriorityTask releases key\n");
        xSemaphoreGive(xMutex);
        printf( "\n");
        /* delay so that lowPriorityTask has chance to run */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

void app_main() {
    esp_task_wdt_deinit();
    xMutex = xSemaphoreCreateMutex();    
    
    xTaskCreate(highPriorityTask, "highPriorityTask", 3000, "High Priority Task", 4, NULL);
    vTaskDelay(pdMS_TO_TICKS(500));
    xTaskCreate(lowPriorityTask, "lowPriorityTask", 3000, "Low Priority Task", 1, NULL);
}
