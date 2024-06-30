#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/uart.h"
#include "esp_log.h"

// Tag for logging
static const char *TAG = "TimerDemo";

// Timer handles
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

// Timer callback function
void myTimerCallback(TimerHandle_t xTimer) {
    if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
        printf("One-shot timer expired\n");
    }
    // Print message if timer 1 expired
    if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
        printf("Auto-reload timer expired\n");
    }
}

// UART initialization function
// void init_uart() {
//     const uart_config_t uart_config = {
//         .baud_rate = 115200,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
//     };
//     uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
//     uart_param_config(UART_NUM_0, &uart_config);
//     uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
// }

// Main application function
void app_main(void) {
    // Initialize UART for logging
    // init_uart();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("---FreeRTOS Timer Demo---\n");

    // Create one-shot timer
    one_shot_timer = xTimerCreate(
                        "One-shot timer",           // Name of timer
                        2000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                        pdFALSE,                    // One-shot
                        (void *)0,                  // Timer ID
                        myTimerCallback);           // Callback function

    // Create auto-reload timer
    auto_reload_timer = xTimerCreate(
                        "Auto-reload timer",        // Name of timer
                        1000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                        pdTRUE,                     // Auto-reload
                        (void *)1,                  // Timer ID
                        myTimerCallback);           // Callback function

    if (one_shot_timer == NULL || auto_reload_timer == NULL) {
        printf("Could not create one of the timers\n");
    } else {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Starting timers...\n");
        xTimerStart(one_shot_timer, portMAX_DELAY);
        xTimerStart(auto_reload_timer, portMAX_DELAY);
    }

    vTaskDelete(NULL);
}
