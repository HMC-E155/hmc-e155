// 03_GPIO.c
// Basic GPIO device driver
// Josh Brake
// jbrake@hmc.edu
// 9/26/22

#include "03_GPIO.h"

// Function to set the mode of a pin to either input, output, alternate function or analog
void pinMode(GPIO * GPIOX, int pin, int function) {
    switch(function) {
        case(GPIO_INPUT):
            // TODO: YOUR CODE HERE
            break;
        // TODO: YOUR CODE HERE Add cases for output, alternate output function (AF), and analog pin modes
    }
}

// Reads and returns the value of an input pin
int digitalRead(GPIO * GPIOX, int pin) {
    // TODO: YOUR CODE HERE
}

// Write a value to a GPIO pin
void digitalWrite(GPIO * GPIOX, int pin, int val) {
    // TODO: YOUR CODE HERE
    
}

// Toggles the state of a GPIO pin (low to high or high to low)
void togglePin(GPIO * GPIOX, int pin) {
    // TODO: YOUR CODE HERE
}
