// STM32F401RE_TIM.h
// Header for TIM functions

#ifndef STM32L4_TIM_H
#define STM32L4_TIM_H

#include <stdint.h> // Include stdint header
#include "STM32L432KC_GPIO.h"

// TODO: Add base address for timer instance
#define TIM15_BASE_ADR ()

// TODO: Create timer typedef structure (TIM_TypeDef) with registers

typedef struct {


} TIM_TypeDef;

#define TIM15 ((TIM_TypeDef *) (TIM15_BASE_ADR))

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initTIM(TIM_TypeDef * TIMx);
void delay_millis(TIM_TypeDef * TIMx, uint32_t ms);
void delay_micros(TIM_TypeDef * TIMx, uint32_t us);

#endif