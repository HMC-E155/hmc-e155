// 01_blink_led.c
// Simple GPIO blink LED with direct pointer manipulation
// Josh Brake
// jbrake@hmc.edu
// 9/26/22

#include <stdint.h>
// Pointer for register to turn on clock for GPIO

// Pointers for registers to configure GPIO (see MCU reference manual)
/* TODO: YOUR CODE HERE */
#define GPIOB_BASE_ADR (0x48000400UL)
#define RCC_BASE_ADR (0x40021000UL)

#define RCC_AHB2ENR ((uint32_t *) (RCC_BASE_ADR + 0x4C))
#define GPIOB_MODER ((uint32_t *) (GPIOB_BASE_ADR + 0x00))
#define GPIOB_ODR ((uint32_t *) (GPIOB_BASE_ADR + 0x14))

// Complete define statements for the LED pin and delay duration
#define LED_PIN         3 // PB3
#define DELAY_DURATION  200

// Simple delay function to use a bunch of NOPs to stall the processor
void delay(int cycles) {
   while (cycles-- > 0) {
      volatile int x=200;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void) {
    // Turn on clock to GPIO
    *RCC_AHB2ENR |= (1 << 1);

    // Set LED_PIN as output
    *GPIOB_MODER &= ~(1 << 7);
    *GPIOB_MODER |= (1 << 6);

    // Blink LED
    while(1) {
        delay(DELAY_DURATION);
        *GPIOB_ODR |= (1 << LED_PIN);
        delay(DELAY_DURATION);
        *GPIOB_ODR &= ~(1 << LED_PIN);
    }
    return 0;
}