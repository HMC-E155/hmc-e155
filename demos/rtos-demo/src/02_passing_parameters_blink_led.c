// 02_passing_parameters_blink_led.c
// This file presents a basic example of passing parameters into a task.
//
// Josh Brake
// jbrake@hmc.edu
// 11/18/20

#include <FreeRTOS.h>
#include "task.h"

///////////////////////////////////////////////////////////////////////////////
// Defines and Macros
///////////////////////////////////////////////////////////////////////////////

#define LED_PIN 3
#define DELAY_MS 1000

///////////////////////////////////////////////////////////////////////////////
// Custom Types
///////////////////////////////////////////////////////////////////////////////

// New type to hold information about LED
typedef struct param_led {
   uint32_t delay_ms;
   uint8_t led_pin;
} param_led;

///////////////////////////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////////////////////////

// Create param_led struct to hold delay and pin number for LED.
param_led led_1 = {DELAY_MS, LED_PIN}; // delay_ms = 200, led_pin = 3

///////////////////////////////////////////////////////////////////////////////
// Configuration Functions
///////////////////////////////////////////////////////////////////////////////

// Initialize GPIO for LED
void init_gpio() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable PortB clock
    GPIOB->MODER &= ~(GPIO_MODER_MODER3);
    GPIOB->MODER |= GPIO_MODER_MODER3_0; // Configure PB3 as output
}

///////////////////////////////////////////////////////////////////////////////
// Helper functions to use in tasks
///////////////////////////////////////////////////////////////////////////////

// Function to toggle LED
void toggleLED(int pin) {
    int pin_state = (GPIOB->IDR >> pin) & 1;

    if (pin_state)
        GPIOB->BSRR = (1 << (pin + 16));
    else
        GPIOB->BSRR = (1 << pin);
}

///////////////////////////////////////////////////////////////////////////////
// Tasks
///////////////////////////////////////////////////////////////////////////////

// Task to toggle LED at 1 Hz
static void toggleLedTask(void *pvParameters) {
    const param_led * led_info = (param_led *) pvParameters;
    const TickType_t xDelay = pdMS_TO_TICKS(led_info->delay_ms);

    while (1)
    {
        /* Simply toggle the LED every xDelay ms, blocking between each toggle. */
        toggleLED(led_info->led_pin);
        vTaskDelay(xDelay);
    }
}

// Main function where initialization is performed and tasks are created.
int main() { 
    // Call initialization functions
    init_gpio();    // Initialize GPIO for LED

    // Create tasks
    const size_t xRegTestStackSize = 250U; // Set value for stack for each task.
    xTaskCreate(toggleLedTask,      // Task function
                "Blink_1",          // Optional name for task
                xRegTestStackSize,  // Task stack size
                (void*)&led_1,      // void pointer to optional parameters
                1,                  // Task priority
                NULL);              // Handle to created task

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite while loop. Should never get here unless the scheduler fails to start.
    while (1);
}
