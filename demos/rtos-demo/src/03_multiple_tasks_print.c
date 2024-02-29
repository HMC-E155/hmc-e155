// 03_multiple_tasks_print.c
// This file presents a basic example of having multiple tasks and printing to
// the terminal over UART.
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
#define DELAY_MS 100
#define USART USART2
#define UART_DELAY_MS 2000
#define HSI_FREQ 16000000

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
param_led led_1 = {DELAY_MS, LED_PIN};

// Strings to print from tasks.
const uint8_t str1[64] = "Hello from Task 1.\n";
const uint8_t str2[64] = "Hello from Task 2.\n";

///////////////////////////////////////////////////////////////////////////////
// Configuration Functions
///////////////////////////////////////////////////////////////////////////////

// Initialize GPIO for LED
void init_gpio() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable PortB clock
    GPIOB->MODER &= ~(GPIO_MODER_MODER3);
    GPIOB->MODER |= GPIO_MODER_MODER3_0; // Configure PB3 as output
}


// Initialize and configure USART
void init_uart() {
    ////////////////////////////////
    // USART Configuration
    ////////////////////////////////

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // Set PA2 to ALT function
    GPIOA->MODER &= ~(GPIO_MODER_MODER2);
    GPIOA->MODER |= _VAL2FLD(GPIO_MODER_MODE2, 0b10);
    
    // Clear ALT function settings
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    
    // Configure correct alternate functions (PA2 to AF07, PA15 to AF03)
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL2,7);
    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL15,3);

    // Configure USART2
    RCC->APB1ENR1 |= (RCC_APB1ENR1_USART2EN); // Set USART2EN
    RCC->CR |= RCC_CR_HSION;  // Turn on HSI 16 MHz clock
    RCC->CCIPR |= _VAL2FLD(RCC_CCIPR_USART2SEL, 2); // Set HSI16 (16 MHz) as USART clock source

    USART->CR1 &= ~(USART_CR1_M); // M=0 corresponds to 8 data bits
    USART->CR2 &= ~(USART_CR2_STOP); // 0b00 corresponds to 1 stop bit
    USART->CR1 &= ~(USART_CR1_OVER8); // Set to 16 times sampling freq

    // Set USART baud rate to 9600

    USART->BRR = (uint16_t) (HSI_FREQ / 9600);
    
    USART->CR1 |= (USART_CR1_UE); // Enable USART
    USART->CR1 |= (USART_CR1_TE | USART_CR1_RE); // Enable USART2
}

// Simple function to send characters over USART.
void sendChar(uint8_t data){
    USART->TDR = _VAL2FLD(USART_TDR_TDR, data);
    while(!(_FLD2VAL(USART_ISR_TC, USART2->ISR) & 1));
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
static void toggleLedTask(void *pvParameters)
{
    const param_led * led_info = (param_led *) pvParameters;
    const TickType_t xDelay = pdMS_TO_TICKS(led_info->delay_ms);

    while (1)
    {
        /* Simply toggle the LED every xDelay ms, blocking between each toggle. */
        toggleLED(led_info->led_pin);
        vTaskDelay(xDelay);
    }
}

// Task to print string over USART
static void printStringTask(void *pvParameters) {
  uint8_t * str = (uint8_t *) pvParameters;
  const TickType_t xDelay = pdMS_TO_TICKS(UART_DELAY_MS);
  int i = 0;

  while(1) {
    do {
      sendChar(str[i]);
      i++;
    }
    while(str[i] != 0);
    
    i = 0;

    vTaskDelay(xDelay);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////

// Main function where initialization is performed and tasks are created.
int main()
{
    // Call initialization functions
    init_gpio();    // Initialize GPIO for LED
    init_uart();    // Initialize UART

    // Create tasks
    const size_t xRegTestStackSize = 250U; // Set value for stack for each task.
    xTaskCreate(toggleLedTask,      // Task function
                "Blink_1",          // Optional name for task
                xRegTestStackSize,  // Task stack size
                (void*)&led_1,      // void pointer to optional parameters
                1,                  // Task priority
                NULL);              // Handle to created task

    xTaskCreate(printStringTask, "Print_Test1", xRegTestStackSize, (void*)&str1, 3, NULL);
    xTaskCreate(printStringTask, "Print_Test2", xRegTestStackSize, (void*)&str2, 4, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite while loop. Should never get here unless the scheduler fails to start.
    while (1);
}
