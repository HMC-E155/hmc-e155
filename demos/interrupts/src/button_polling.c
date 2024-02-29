// button_polling.c
// Josh Brake
// jbrake@hmc.edu
// 10/31/22

/*
  This program polls the user button on the Nucleo-L432KC board and has a
  delay within the main loop to simulate the problems with polling for 
  catching events.
*/

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

    int volatile cur_button_state = digitalRead(BUTTON_PIN);
    int volatile led_state = 0;
    int volatile prev_button_state = cur_button_state;

    while(1){
        prev_button_state = cur_button_state;
        cur_button_state = digitalRead(BUTTON_PIN);
        if ((prev_button_state == 1) && (cur_button_state == 0)) {
            led_state = !led_state;
            digitalWrite(LED_PIN, led_state);
        }
        delay_millis(DELAY_TIM, 200);
    }
}
