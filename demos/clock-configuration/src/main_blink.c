// main_blink.c
// GPIO blink LED with clock configuration
// Josh Brake
// jbrake@hmc.edu
// 9/28/21

// TODO: Add #includes

#define LED_PIN           3
#define DELAY_DURATION    500

void delay(int ms) {
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void) {
    configureFlash();

    // Call configureClock() (declared in STM32F401RE_RCC.h)
    configureClock();

    // Turn on clock to GPIOB
    RCC->AHB2ENR |= (1 << 1):;

    // Set LED_PIN as output
    pinMode(LED_PIN, GPIO_OUTPUT);

    // Blink LED
    while(1) {
        ms_delay(DELAY_MS);
        togglePin(LED_PIN);
    }
    return 0;
}