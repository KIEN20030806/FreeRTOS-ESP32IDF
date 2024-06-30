#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"

void task1(void *param);
void task2(void *param);

TaskHandle_t TaskHandle_1 = NULL;
TaskHandle_t TaskHandle_2 = NULL;

QueueHandle_t Queue;

void task1(void *p) {
    char myTxBuff[30];
    // Tạo hàng đợi nếu chưa tạo
    if (Queue == NULL) {
        Queue = xQueueCreate(7, sizeof(myTxBuff));
    }

    // Ghi Tek4VN vào mảng myTxBuff
    sprintf(myTxBuff, "Tek4VN");
    xQueueSend(Queue, (void*) myTxBuff, (TickType_t) 0); // gửi vào hàng đợi
    sprintf(myTxBuff, "https://tek4.vn/");
    xQueueSend(Queue, (void*) myTxBuff, (TickType_t) 0);
    sprintf(myTxBuff, "ESP32-FreeRTOS");
    xQueueSend(Queue, (void*) myTxBuff, (TickType_t) 0);
    sprintf(myTxBuff, "Xin chao cac ban");
    xQueueSend(Queue, (void*) myTxBuff, (TickType_t) 0);

    printf("data waiting to be read: %d\n", uxQueueMessagesWaiting(Queue)); // các dữ liệu đang đợi để đọc
    printf("available spaces: %d\n", uxQueueSpacesAvailable(Queue)); // kiểm tra không gian còn trống

    while (true) {
        vTaskDelay(portMAX_DELAY);
    }
}

void task2(void *p) {
    char myRxBuff[30];
    while (true) {
        if (Queue != NULL) { // nếu queue khác NULL thì sẽ nhận dữ liệu từ hàng đợi
            if (xQueueReceive(Queue, (void*) myRxBuff, (TickType_t) 5)) {
                printf("data receive: %s\n", myRxBuff);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    // Tạo hàng đợi trước khi tạo task
    Queue = xQueueCreate(7, sizeof(char[30]));

    // Kiểm tra xem hàng đợi có được tạo thành công không
    if (Queue == NULL) {
        printf("Failed to create queue\n");
        return;
    }

    // Tạo task task1 và task2
    xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 1, &TaskHandle_1, 1);
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 1, &TaskHandle_2, 1);
}
