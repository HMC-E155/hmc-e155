// STM32L432KC_RCC.c
// Source code for RCC functions

#include "STM32L432KC_RCC.h"

void configurePLL() {
    // Set clock to 80 MHz
    // Output freq = (src_clk) * (N/M) / R
    // (4 MHz) * (80/1) / 4 = 80 MHz
    // M:2, N:80, R:2
    // Use MSI as PLLSRC

    RCC->CR &= ~(1 << 24); // Turn off PLL
    while ((RCC->CR >> 25 & 1) != 0); // Wait till PLL is unlocked (e.g., off)

    // Load configuration
    // Set PLL SRC to MSI
    RCC->PLLCFGR |= (1 << 0); 
    RCC->PLLCFGR &= ~(1 << 1); 
    
    // Set PLLM to 1
    RCC->PLLCFGR &= ~(0b111 << 4);
    RCC->PLLCFGR |= (0b000 << 4);

    // Set PLLN to 80
    RCC->PLLCFGR &= ~(0b11111111 << 8); // Clear all bits of PLLN
    RCC->PLLCFGR |= (80 << 8);    // Set PLLN to 80

    // Set PLLR to 4
    RCC->PLLCFGR &= ~(0b11 << 25); 
    RCC->PLLCFGR |= (0b01 << 25); 
    
    // Enable PLLR output
    RCC->PLLCFGR |= (1 << 24);

    // Enable PLL and wait until it's locked
    RCC->CR |= (1 << 24);
    
    while(!((RCC->CR >> 25) & 1));
}

void configureClock(){
    // Configure and turn on PLL
    configurePLL();

    // Select PLL as clock source
    RCC->CFGR |= (0b11 << 0);
    while(!((RCC->CFGR >> 2) & 0b11));
}