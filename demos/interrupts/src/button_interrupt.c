// button_interrupt.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22

#include "main.h"

int main(void) {
    // Enable LED as output
    gpioEnable(GPIO_PORT_B);
    pinMode(LED_PIN, GPIO_OUTPUT);

    // Enable button as input
    gpioEnable(GPIO_PORT_A);
    pinMode(BUTTON_PIN, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b01); // Set PA2 as pull-up

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    // 2. Configure EXTICR for the input button interrupt

    // Enable interrupts globally
    __enable_irq();

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // 1. Configure mask bit
    // 2. Disable rising edge trigger
    // 3. Enable falling edge trigger
    // 4. Turn on EXTI interrupt in NVIC_ISER

    while(1){   
        delay_millis(TIM2, 200);
    }

}

// TODO: What is the right name for the IRQHandler?
void XXXXXX(void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << )){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << );

        // Then toggle the LED
        togglePin(LED_PIN);

    }
}
