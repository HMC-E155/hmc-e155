// STM32L432KC_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
    // Set clock to 80 MHz
    // Output freq = (src_clk) * (N/M) / R
    // (4 MHz) * (N/M) / R = 80 MHz
    // M: XX, N: XX, R: XX
    // Use MSI as PLLSRC

    // TODO: Turn off PLL
    RCC->CR &= 
    // TODO: Wait till PLL is unlocked (e.g., off)
    while ((RCC->CR >> 25 & 1) != 0);

    // Load configuration
    // TODO: Set PLL SRC to MSI
    RCC->PLLCFGR |= 
    RCC->PLLCFGR &= 
    // TODO: Set PLLN
    RCC->PLLCFGR &= ~(0b11111111 << 8); // Clear all bits of PLLN
    RCC->PLLCFGR |= 
    // TODO: Set PLLM
    RCC->PLLCFGR &= ~(0b111 << 4);  // Clear all bits
    RCC->PLLCFGR |= 

    // TODO: Set PLLR
    RCC->PLLCFGR &= 
    
    // TODO: Enable PLLR output
    RCC->PLLCFGR |= 

    // TODO: Enable PLL
    RCC->CR |= 
    
    // TODO: Wait until PLL is locked
    while();
}

void configureClock(){
    // Configure and turn on PLL
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR |= (0b11 << 0);
    while(!((RCC->CFGR >> 2) & 0b11));
}