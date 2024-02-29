// STM32L432KC.h
// Header to include all other STM32L432KC libraries.

#ifndef STM32L4_H
#define STM32L4_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stm32l432xx.h>  // CMSIS device library include

// Include other peripheral libraries

#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_TIM.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_USART.h"
#include "STM32L432KC_SPI.h"

// Global defines

#define HSI_FREQ 16000000 // HSI clock is 16 MHz
#define MSI_FREQ 4000000  // HSI clock is 4 MHz


#endif