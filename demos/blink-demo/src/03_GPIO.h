// 03_GPIO.h
// Basic GPIO header
// Josh Brake
// jbrake@hmc.edu
// 9/26/22

#ifndef STM32L4_GPIO_H
#define STM32L4_GPIO_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// Values for GPIO pins ("val" arguments)
#define GPIO_LOW    0
#define GPIO_HIGH   1

// // Base addresses for GPIO ports
#define GPIOA_BASE  (0x40020000UL)
#define GPIOB_BASE  // TODO: YOUR CODE HERE
#define GPIOC_BASE  // TODO: YOUR CODE HERE

// Arbitrary GPIO functions for pinMode()
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1
#define GPIO_ALT    2
#define GPIO_ANALOG 3

///////////////////////////////////////////////////////////////////////////////
// Bitfield structs
///////////////////////////////////////////////////////////////////////////////

// GPIO register structs here
typedef struct {
    volatile uint32_t MODER;   // GPIO Offset 0x00 GPIO port mode register
    volatile uint32_t OTYPER;  // GPIO Offset 0x04
    volatile uint32_t OSPEEDR; // GPIO Offset 0x08
    volatile uint32_t PURPDR;  // GPIO Offset 0x0C
    volatile uint32_t IDR;     // GPIO Offset 0x10
    volatile uint32_t ODR;     // GPIO Offset 0x14
    volatile uint32_t BSRR;    // GPIO Offset 0x18
    volatile uint32_t LCKR;    // GPIO Offset 0x1C
    volatile uint32_t AFRL;    // GPIO Offset 0x20
    volatile uint32_t AFRH;    // GPIO Offset 0x24
} GPIO;

// Pointers to GPIO-sized chunks of memory for each peripheral
#define GPIOA ((GPIO *) GPIOA_BASE)
#define GPIOB // TODO: YOUR CODE HERE
#define GPIOC // TODO: YOUR CODE HERE

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

// Function to set the mode of a pin to either input, output, alternate function or analog
void pinMode(GPIO * GPIOX, int pin, int function);

// Reads and returns the value of an input pin
int digitalRead(GPIO * GPIOX, int pin);

// Write a value to a GPIO pin
void digitalWrite(GPIO * GPIOX, int pin, int val);

// Toggles the state of a GPIO pin (low to high or high to low)
void togglePin(GPIO * GPIOX, int pin);

#endif