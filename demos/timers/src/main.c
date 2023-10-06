// Basic timer demo
// Josh Brake
// jbrake@hmc.edu
// 9/28/22


#include "STM32L432KC_TIM.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_FLASH.h"


#define LED_PIN 3

int main() {

  // Turn on clock to GPIOB
  RCC->AHB2ENR |= (1 << 1);

  // Turn on clock to TIM15
  RCC->APB2ENR|= (1 << 16);

  // Set LED_PIN as output
  pinMode(LED_PIN, GPIO_OUTPUT);

  initTIM(TIM15);


  // Blink LED
  while(1) {
      delay_millis(TIM15, 500);
      togglePin(LED_PIN);
  }
  return 0;
}