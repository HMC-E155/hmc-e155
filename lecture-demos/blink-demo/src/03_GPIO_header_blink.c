// 03_GPIO_header_blink.c
// Blink LED with program using header
// Josh Brake
// jbrake@hmc.edu
// 9/22/21

#include "03_GPIO.h"

// Pointer to AHB2 enable register for turning on GPIOB
volatile unsigned long * RCC_AHB1ENR  = (unsigned long *) (0x40023830UL);

#define LED_PIN           3   // PB3
#define BUTTON_PIN        2   // PA2
#define DELAY_DURATION    100

void delay(int cycles) {
   while (cycles-- > 0) {
      volatile int x=200;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void) {
    // Turn on clock to GPIOA and GPIOC using pointer manipulation
    // TODO: YOUR CODE HERE

    // Set LED_PIN as output using pinMode()
    // TODO: YOUR CODE HERE

    // Set button pin as input using pinMode()
    // TODO: YOUR CODE HERE

    // Blink LED using digitalRead() and digitalWrite()
    while(1) {
      // TODO: YOUR CODE HERE
    }
    return 0;
}