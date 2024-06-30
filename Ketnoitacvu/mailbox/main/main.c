#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Define the queue handle and task handles
QueueHandle_t xMailbox;
TaskHandle_t TaskHandle_1; // handler for Task1
TaskHandle_t TaskHandle_2; // handler for Task2

void vUpdateMailbox(void *pvParameters)
{
    int32_t ulNewValue = 1;
    while (1)
    {
        xQueueOverwrite(xMailbox, &ulNewValue); // Overwrite value ulNewValue into the queue
        printf("Ghi giá trị vào Mailbox\n");
        ulNewValue++;
        if (ulNewValue > 100) ulNewValue = 0;
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000ms
    }
}

void vReadMailbox(void *pvParameters)
{
    int value_received;
    while (1)
    {
        xQueuePeek(xMailbox, &value_received, portMAX_DELAY);
        printf("Giá trị đọc được từ mailbox = %d\n", value_received);
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
    }
}

void app_main(void)
{
    // Create the mailbox (queue)
    xMailbox = xQueueCreate(1, sizeof(int32_t));
    
    if (xMailbox == NULL)
    {
        printf("Failed to create mailbox\n");
        return;
    }

    // Create the tasks
    xTaskCreate(vUpdateMailbox, "Sender", 2048, NULL, 1, &TaskHandle_1);
    xTaskCreate(vReadMailbox, "Receiver", 2048, NULL, 1, &TaskHandle_2);
}
