/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
// Include the device header
#include <stm32l432xx.h>

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

int main(void) {
  // Initialization code

  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

  // Set PA6 and PA10 as an output
  GPIOA->MODER |=  (GPIO_MODER_MODE10_0);
  GPIOA->MODER &= ~(GPIO_MODER_MODE10_1);

  // Set PA9 as an input
  GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
  GPIOA->MODER &= ~GPIO_MODER_MODE9_1;

  // Initialization code
  // Read input on PA9 and PA10 pin accordingly.
  while(1) {
    if ((GPIOA->IDR >> 9) & 0x1) {
      GPIOA->ODR |= (1 << 10);
      }
    else {
      GPIOA->ODR &= ~(1 << 10);
    }
  }
}

/*************************** End of file ****************************/
