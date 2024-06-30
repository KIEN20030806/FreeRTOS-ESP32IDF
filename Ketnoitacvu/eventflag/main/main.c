#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Declare the event group
EventGroupHandle_t event_group;

// Define the bit flag
#define BUTTON_PRESSED_FLAG (1 << 0)

// Task 1: Simulate the button press "Change light"
void task_button(void *pvParameters)
{
    while (true) {
        // Wait for 2 seconds
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        // Set the event flag "BUTTON_PRESSED_FLAG"
        xEventGroupSetBits(event_group, BUTTON_PRESSED_FLAG);

        // Print a message
        printf("Nút bấm được nhấn!\n");
    }
}

// Task 2: Manage the traffic light state
void task_traffic_light(void *pvParameters)
{
    uint8_t current_state = 0; // 0: Red, 1: Yellow, 2: Green

    while (true) {
        // Wait for the event flag "BUTTON_PRESSED_FLAG"
        xEventGroupWaitBits(event_group, BUTTON_PRESSED_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);

        // Update the light state
        current_state = (current_state + 1) % 3;

        // Print the light state
        switch (current_state) {
            case 0:
                printf("Đèn đỏ!\n");
                break;
            case 1:
                printf("Đèn vàng!\n");
                break;
            case 2:
                printf("Đèn xanh!\n");
                break;
        }

        // Display the light state (using LEDs or printing to the screen)
        // ...

        // Wait for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Task 3: Display the traffic light state (if needed)
/*
void task_display(void *pvParameters)
{
    while (true) {
        // Display the light state (using LEDs or printing to the screen)
        // ...

        // Wait for 0.5 seconds
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
*/

void app_main(void)
{
    // Create the event group
    event_group = xEventGroupCreate();

    // Create the tasks
    xTaskCreate(task_button, "Button", 1024, NULL, 1, NULL);
    xTaskCreate(task_traffic_light, "Traffic Light", 1024, NULL, 2, NULL);
    // xTaskCreate(task_display, "Display", 1024, NULL, 3, NULL);
}
