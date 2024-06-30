#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
int counter = 0;

void Task_Print1(void *param);
void Task_Print2(void *param);

void app_main()
{
    // Initialize serial communication
    printf("Task Suspend and Resume ESP-IDF Example\n");

    // Create two tasks
    xTaskCreate(Task_Print1, "Task1", 2048, NULL, 1, &TaskHandle_1);
    xTaskCreate(Task_Print2, "Task2", 2048, NULL, 1, &TaskHandle_2);
}

void Task_Print1(void *param)
{
    (void)param;
    TickType_t getTick;
    getTick = xTaskGetTickCount();

    while(1)
    {
        printf("TASK 1\n");
        if (counter == 21)
        {
            vTaskDelete(TaskHandle_1);
        }
        counter++;
        if (counter == 15)
        {
            vTaskResume(TaskHandle_2); // Tiếp tục Task 2
        }
        vTaskDelayUntil(&getTick, 1000 / portTICK_PERIOD_MS);
    }
}

void Task_Print2(void *param)
{
    (void)param;
    while(1)
    {
        printf("TASK 2\n");
        counter++;
        if (counter == 10)
        {
            vTaskSuspend(TaskHandle_2); // Tạm dừng Task 2
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
