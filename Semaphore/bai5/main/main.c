#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define SEMAPHORE_MAX_COUNT 1
#define SEMAPHORE_INITIAL_COUNT 0

int nTasks = 4;
SemaphoreHandle_t barrierSemaphore;

void genericTask(void *parameter) {
    int taskNumber = (int)parameter;
    char taskMessage[50];
    sprintf(taskMessage, "Task number: %d", taskNumber);
    printf("%s\n", taskMessage);
    xSemaphoreGive(barrierSemaphore);
    vTaskDelete(NULL);
}

void app_main() {
    printf("Starting to launch tasks..\n");
    StaticSemaphore_t barrierSemaphoreBuffer;
    barrierSemaphore = xQueueCreateCountingSemaphore(SEMAPHORE_MAX_COUNT, SEMAPHORE_INITIAL_COUNT);
    int i;
    for (i = 0; i < nTasks; i++) {
        xTaskCreatePinnedToCore(
            genericTask,      /* Function to implement the task */
            "genericTask",    /* Name of the task */
            10000,            /* Stack size in words */
            (void *)i,        /* Task input parameter */
            1,                /* Priority of the task */
            NULL,             /* Task handle */
            1);               /* Core where the task should run */
    }

    for (i = 0; i < nTasks; i++) {
        xSemaphoreTake(barrierSemaphore, portMAX_DELAY);
    }
    printf("Tasks launched and semaphore passed...\n");
}
