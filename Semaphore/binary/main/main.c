//////binary//////////
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "esp_task_wdt.h"

SemaphoreHandle_t Sem_Handle;

void task1(void*) {
  TaskHandle_t tsk1;
  while(1) {
    tsk1 = xTaskGetCurrentTaskHandle();
    printf("TASK1 give semaphore\n");
    xSemaphoreGive(Sem_Handle);
    printf("TASK1 continute ...\n");
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task2(void*) {
  TaskHandle_t tsk2;
  while(1) {
    tsk2 = xTaskGetCurrentTaskHandle();
    if (xSemaphoreTake(Sem_Handle, portMAX_DELAY) == pdTRUE) {
      printf("TASK2 dong bo semaphore\n");
      // Thực hiện phần tới quan trọng ở đây
    }
  }
}


void app_main() {
  esp_task_wdt_deinit();
  Sem_Handle = xSemaphoreCreateBinary();
  xTaskCreate(task1, "TASK1", 1000, NULL, 1, NULL);
  xTaskCreate(task2, "TASK2", 1000, NULL, 2, NULL);
}
