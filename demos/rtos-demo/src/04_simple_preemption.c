// 04_simple_preemption.c
// This file presents a simple example of preemption using a task to print to 
// the terminal over UART, slowly blink an LED in idle, and rapidly blink an
// LED when a button is pressed.
//
// Josh Brake
// jbrake@hmc.edu
// 11/18/20

#include <FreeRTOS.h>
#include "task.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Defines and Macros
///////////////////////////////////////////////////////////////////////////////

#define LED_PIN 3
#define BUTTON_PIN 2
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

///////////////////////////////////////////////////////////////////////////////
// Configuration Functions
///////////////////////////////////////////////////////////////////////////////

// Initialize GPIO for LED
void init_gpio() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable PortB clock
    GPIOB->MODER &= ~(GPIO_MODER_MODER3);
    GPIOB->MODER |= GPIO_MODER_MODER3_0; // Configure PB3 as output
}

// Initialize GPIO for LED
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
  char * str = (char *) pvParameters;
  char str_temp[64];
  const TickType_t xDelay = pdMS_TO_TICKS(UART_DELAY_MS);
  int i = 0;
  int iteration_num = 0;

  while(1) {
    sprintf(str_temp, "%s Iteration num = %d\n", str, iteration_num);
    do {
      sendChar(str_temp[i]);
      i++;
    }
    while(str_temp[i] != 0);
    
    i = 0;
    iteration_num += 1;
    vTaskDelay(xDelay);
  }
}

// Task to poll button
static void pollButtonTask(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(100); // Schedule every 100 ms
  volatile int i;

  while(1) {
    volatile int pin_val = 1;
    
    // Loop to check if the button is pressed (button is pulled low when pressed)
    // and blink LED rapidly while the button is pressed using a dummy loop.
    while(pin_val){
      pin_val = !((GPIOA->IDR >> BUTTON_PIN) & 1);
      if(pin_val) {
        toggleLED(LED_PIN);
        for(i=0; i < 10000; i++); // Dummy loop to do a delay.
      }
    }
    vTaskDelay(xDelay);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////

// Main function where initialization is performed and tasks are created.
int main() {
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

    xTaskCreate(printStringTask, "Print_Test1", xRegTestStackSize, (void*)&str1, 2, NULL);
    xTaskCreate(pollButtonTask, "Poll_Button", xRegTestStackSize, NULL, 3, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite while loop. Should never get here unless the scheduler fails to start.
    while (1);
}
