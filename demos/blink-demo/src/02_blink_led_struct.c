// 02_blink_led_struct.c
// Blnk LED with structure
// Josh Brake
// jbrake@hmc.edu
// 9/26/22

// Include integer types with specified widths like uint32_t
#include <stdint.h>

// Pointer to AHB2 enable register for turning on GPIOB
volatile unsigned long * RCC_AHB2ENR  = (unsigned long *) (0x4002104CUL);

// Base addresses for GPIO ports
#define GPIOB_BASE (0x48000400UL)

// GPIO register structs here
typedef struct {
  /* TODO: YOUR CODE HERE */
} GPIO;

// Pointers to GPIO-sized chunks of memory for each peripheral
#define GPIOB ((GPIO *) GPIOB_BASE)

#define LED_PIN     3
#define DELAY_DURATION    100

void delay(int cycles) {
   while (cycles-- > 0) {
      volatile int x=200;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void) {
    // Turn on clock for GPIO
    *RCC_AHB2ENR |= (1 << 1);

    // Set LED_PIN as output (two different, equivalent ways to modify the registers. -> is preferred.)
    (*GPIOB).MODER |= (1 << 2*LED_PIN);
    GPIOB->MODER &= ~(1 << (2*LED_PIN+1));


    // Blink LED
    while(1) {
        delay(DELAY_DURATION);

        if((GPIOB->ODR >> LED_PIN) & 1){
          GPIOB->ODR &= ~(1 << LED_PIN);
        }
        else{
          GPIOB->ODR |= (1 << LED_PIN);
          }
    }
    return 0;
}