// STM32F401RE_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
   // Set clock to 80 MHz
   // Output freq = (src_clk) * (N/M) / P
   // (4 MHz) * (80/2) * 2  = 80 MHz
   // M:, N:, P:
   // Use HSI as PLLSRC

   RCC->CR &= ~_FLD2VAL(RCC_CR_PLLON, RCC->CR); // Turn off PLL
   while (_FLD2VAL(RCC_CR_PLLRDY, 1) != 0); // Wait till PLL is unlocked (e.g., off)

   // Load configuration
   RCC->PLLCFGR |= _VAL2FLD(RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_MSI);
   RCC->PLLCFGR |= _VAL2FLD(RCC_PLLCFGR_PLLM, 0b001); // M = 2
   RCC->PLLCFGR |= _VAL2FLD(RCC_PLLCFGR_PLLN, 80);    // N = 80
   RCC->PLLCFGR |= _VAL2FLD(RCC_PLLCFGR_PLLR, 0b00);  // R = 2
   RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;                // Enable PLLCLK output

   // Enable PLL and wait until it's locked
   RCC->CR |= RCC_CR_PLLON;
   while(_FLD2VAL(RCC_CR_PLLRDY, RCC->CR) == 0);
}

void configureClock(){
  // Configure and turn on PLL
  configurePLL();

  // Select PLL as clock source
  RCC->CFGR = RCC_CFGR_SW_PLL | (RCC->CFGR & ~RCC_CFGR_SW);
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

  SystemCoreClockUpdate();
}