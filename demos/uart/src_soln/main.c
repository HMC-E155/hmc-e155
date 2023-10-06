// Lecture 12 Demo
// Josh Brake
// jbrake@hmc.edu
// 10/5/22

#include "STM32L432KC.h"
#include <stm32l432xx.h>
#define USART_ID USART2_ID
#define TIM TIM15

int main(void) {
// Configure flash and clock
configureFlash();
configureClock();

// Initialize USART
USART_TypeDef * USART = initUSART(USART_ID, 9600);

// Initialize timer
RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
initTIM(TIM);

char msg[28] = "Happy Hacking!\n\r";


while(1){
  int i = 0;
  do {
    sendChar(USART, msg[i]);
    i += 1;
  } while (msg[i]);
  delay_millis(TIM, 2000);
  }
}
