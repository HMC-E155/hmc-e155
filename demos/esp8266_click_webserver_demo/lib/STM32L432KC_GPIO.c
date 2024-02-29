// STM32L432KC_GPIO.c
// Source code for GPIO functions

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// PIO Helper Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

void gpioEnable(int port_id) {
  switch (port_id) {
    case GPIO_PORT_A:
      RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
      break;
    case GPIO_PORT_B:
      RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
      break;
    case GPIO_PORT_C:
      RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
      break;
  }
}

/* Returns the port ID that corresponds to a given pin.
 *    -- pin: a GPIO pin ID, e.g. PA3
 *    -- return: a GPIO port ID, e.g. GPIO_PORT_ID_A */
int gpioPinOffset(int gpio_pin) {
  // Returns offset of pin within port (given by 4 least significant bits)
  return gpio_pin & 0x0F;
}

/* Returns the port ID that corresponds to a given pin.
 *    -- pin: a GPIO pin ID, e.g. PA3
 *    -- return: a GPIO port ID, e.g. GPIO_PORT_ID_A */
int gpioPinToPort(int gpio_pin) {
  // Shift to the right by 4 bits since there are 16 (2^4) pins per port
  return gpio_pin >> 4;
}

/* Returns a pointer to the given port's base address.
 *    -- port: a GPIO port ID, e.g. GPIO_PORT_ID_A
 *    -- return: a pointer to a gpio-sized block of memory at the port "port" */
GPIO_TypeDef * gpioPortToBase(int port) {
  GPIO_TypeDef * port_id = 0x0;
  switch (port) {
    case GPIO_PORT_A:
      port_id = (GPIO_TypeDef *) GPIOA_BASE;
      break;
    case GPIO_PORT_B:
      port_id = (GPIO_TypeDef *) GPIOB_BASE;
      break;
    case GPIO_PORT_C:
      port_id = (GPIO_TypeDef *) GPIOC_BASE;
      break;
  }
  return port_id;
}

/* Given a pin, returns a pointer to the corresponding port's base address.
 *    -- pin: a PIO pin ID, e.g. PIO_PA3
 *    -- return: a pointer to a Pio-sized block of memory at the pin's port */
GPIO_TypeDef * gpioPinToBase(int gpio_pin) {
  return gpioPortToBase(gpioPinToPort(gpio_pin));
}


void pinMode(int gpio_pin, int function) {
	// Get pointer to base address of the corresponding GPIO pin and pin offset
	GPIO_TypeDef * GPIO_PORT_PTR = gpioPinToBase(gpio_pin);
	int pin_offset = gpioPinOffset(gpio_pin);

	switch(function) {
		case GPIO_INPUT:
			GPIO_PORT_PTR->MODER &= ~(0b11 << 2*pin_offset);
			break;
		case GPIO_OUTPUT:
			GPIO_PORT_PTR->MODER |= (0b1 << 2*pin_offset);
			GPIO_PORT_PTR->MODER &= ~(0b1 << (2*pin_offset+1));
			break;
		case GPIO_ALT:
			GPIO_PORT_PTR->MODER &= ~(0b1 << 2*pin_offset);
			GPIO_PORT_PTR->MODER |= (0b1 << (2*pin_offset+1));
			break;
		case GPIO_ANALOG:
			GPIO_PORT_PTR->MODER |= (0b11 << 2*pin_offset);
			break;
	}
}

int digitalRead(int gpio_pin) {
	// Get pointer to base address of the corresponding GPIO pin and pin offset
	GPIO_TypeDef * GPIO_PORT_PTR = gpioPinToBase(gpio_pin);
	int pin_offset = gpioPinOffset(gpio_pin);

	return ((GPIO_PORT_PTR->IDR) >> pin_offset) & 1;
}

void digitalWrite(int gpio_pin, int val) {
	// Get pointer to base address of the corresponding GPIO pin and pin offset
	GPIO_TypeDef * GPIO_PORT_PTR = gpioPinToBase(gpio_pin);
	int pin_offset = gpioPinOffset(gpio_pin);

	if (val == 1) {
		GPIO_PORT_PTR->ODR |= (1 << pin_offset);
	}
	else if (val == 0) {
		GPIO_PORT_PTR->ODR &= ~(1 << pin_offset);
	}
	
}

void togglePin(int gpio_pin) {
	// Get pointer to base address of the corresponding GPIO pin and pin offset
	GPIO_TypeDef * GPIO_PORT_PTR = gpioPinToBase(gpio_pin);
	int pin_offset = gpioPinOffset(gpio_pin);

	// Use XOR to toggle
	GPIO_PORT_PTR->ODR ^= (1 << pin_offset);
}