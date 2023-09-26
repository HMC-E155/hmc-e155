# Clock Configuration Demo

This folder contains partially completed code snippets to configure the PLL.
The basic procedure for changing the clock speed is as follows:
1. Adjust the number of waitstates for the flash as required (see reference manual for details on what the latency needs to be set to for a given clock speed and core voltage).
2. Configure the PLL to set the desired clock frequency by configuring the appropriate values for:
   * The input clock source
   * The dividers (M, R) and multipliers (N)
3. Enable the PLL output
4. Once the PLL is locked, swap the system clock source to the PLL.

In the provided code the code to blink the LED and to properly configure the flash memory latency has already been completed for you.
You need to choose the proper values for the PLL configuration in the `configurePLL()` function in `STM32L432KC_RCC.h`.
(Note that on the STM32L432KC, the clock speed should not exceed 80 MHz).
