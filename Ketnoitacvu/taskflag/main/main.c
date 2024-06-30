#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Task handles
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

// Task notification variable
uint32_t ulTaskNotification;

void vTask1(void *pvParameters)
{  
    // Thêm độ trễ để đảm bảo vTask2 đã khởi tạo
    vTaskDelay(pdMS_TO_TICKS(100)); 
    
    while (true)
    {
        printf("Tạo sản phẩm\n");
        // Set task notification bit
        xTaskNotify(xTask2Handle, 0x01, eSetBits);
        // Simulate some delay in product creation
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    while (true)
    {
        // Wait for task notification
        ulTaskNotification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (ulTaskNotification & 0x01)
        {
            printf("Đã nhận sản phẩm\n");
        }
    }
}

void app_main(void)
{
    // Create tasks
    xTaskCreatePinnedToCore(vTask1, "Task1", 2048, NULL, 1, &xTask1Handle, 0);
    xTaskCreatePinnedToCore(vTask2, "Task2", 2048, NULL, 1, &xTask2Handle, 0);
}
