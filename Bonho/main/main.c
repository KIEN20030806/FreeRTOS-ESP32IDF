#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_heap_caps.h"

// Tag for logging
static const char *TAG = "MemoryDemo";

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Task: Perform some mundane task
void testTask(void *parameter) {
    while (1) {
        int a = 1;
        int b[100];  // 400 bytes size

        // Do something with array so it's not optimized out by the compiler
        for (int i = 0; i < 100; i++) {
            b[i] = a + 1;
        }
        printf("%d\n", b[0]);

        // Print out remaining stack memory (words)
        printf("High water mark: %d\n", uxTaskGetStackHighWaterMark(NULL));

        // Print out number of free heap memory bytes before malloc
        printf("Heap before malloc (bytes): %d\n", xPortGetFreeHeapSize());

        int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));

        // One way to prevent heap overflow is to check the malloc output
        if (ptr == NULL) {
            printf("Not enough heap.\n");
            vPortFree(NULL);
        } else {
            // Do something with the memory so it's not optimized out by the compiler
            for (int i = 0; i < 1024; i++) {
                ptr[i] = 3;
            }
        }

        // Print out number of free heap memory bytes after malloc
        printf("Heap after malloc (bytes): %d\n", xPortGetFreeHeapSize());

        // Free up our allocated memory
        vPortFree(ptr);

        // Wait for a while
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    // Configure logging
    // esp_log_level_set(TAG, ESP_LOG_INFO);

    // Wait a moment to start (so we don't miss logging output)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("---FreeRTOS Memory Demo---\n");

    // Start the only other task
    xTaskCreatePinnedToCore(testTask, //768 bytes
                          "Test Task",
                          2500,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

    // Delete "setup and loop" task
    vTaskDelete(NULL);
}