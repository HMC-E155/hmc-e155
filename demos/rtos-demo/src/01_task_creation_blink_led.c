// 01_task_creation_blink_led.c
// This file presents an example of basic task creation to blink an LED.
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

// Task to toggle LED
static void toggleLedTask(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(200);

    while (1) {
        /* Simply toggle the LED every xDelay ms, blocking between each toggle. */
        toggleLED(LED_PIN);
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
                NULL,               // void pointer to optional parameters
                1,                  // Task priority
                NULL);              // Handle to created task

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite while loop. Should never get here unless the scheduler fails to start.
    while (1);
}
