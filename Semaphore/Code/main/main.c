////////code////////////
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#define configUSE_COUNTING_SEMAPHORES 1
#define configUSE_PREEMPTION 1

void task1(void *param);
void task2(void *param);
void task3(void *param);

TaskHandle_t TaskHandle1;
TaskHandle_t TaskHandle2;
TaskHandle_t TaskHandle3;

SemaphoreHandle_t Sema_handle;

void task1(void *p) {
    while (1) {
        printf("task1 give 1 Semaphore\n");
        xSemaphoreGive(Sema_handle); // Sema++
        vTaskDelay(pdMS_TO_TICKS(100)); // Task1 đi ngủ, task2 sẽ được chạy
    }
}

void task2(void *p) {
    while (1) {
        printf("task2 give 1 Semaphore\n");
        xSemaphoreGive(Sema_handle); // Sema++
        vTaskDelay(pdMS_TO_TICKS(100)); // Task2 đi ngủ, task1 sẽ được chạy
    }
}

void task3(void *p) {
    while (1) {
        xSemaphoreTake(Sema_handle, portMAX_DELAY); // Lấy semaphore
        xSemaphoreTake(Sema_handle, portMAX_DELAY); // Lấy semaphore tiếp
        printf("task 3 take 2 Semaphore\n");
    }
}

void app_main() {
    Sema_handle = xSemaphoreCreateCounting(2, 0); // Tạo semaphore đếm với max count = 2 và initial count = 0
    if (Sema_handle == NULL) {
        printf("Failed to create semaphore\n");
        return;
    }

    xTaskCreatePinnedToCore(task1, "task1", 1024, NULL, 1, &TaskHandle1, 1);
    xTaskCreatePinnedToCore(task2, "task2", 1024, NULL, 1, &TaskHandle2, 1);
    xTaskCreatePinnedToCore(task3, "task3", 1024, NULL, 2, &TaskHandle3, 1);
}
