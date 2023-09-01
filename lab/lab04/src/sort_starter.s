// sort_starter.s
// Main sort function template
// jbrake@hmc.edu
// 6/23/20

// Directives 
.syntax unified // Specify the syntax for the file
.cpu cortex-m4  // Target CPU is Cortex-M4
.fpu softvfp    // Use software libraries for floating-point operations
.thumb          // Instructions should be encoded as Thumb instructions

// Define main globally for other files to call
.global main

// Create test array of bytes. Change this for different test cases.
// This will get loaded to the RAM by the startup code
.data
arr:
  .byte 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
.size arr, .-arr

.text

// The main function
.type main, %function
main:
  ldr r3, =arr // Load the base address of RAM where the array is stored
  // YOUR CODE HERE
  
  
.size main, .-main
