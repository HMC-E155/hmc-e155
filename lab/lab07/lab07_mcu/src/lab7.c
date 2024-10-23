/**
    Main: Contains main function for AES SPI communication with FPGA.
    Sends the plaintext and key over SPI to the FPGA and then receives back
    the cyphertext. The cyphertext is then compared against the solution
    listed in Appendix A of the AES FIPS 197 standard.
    @file lab7.c
    @author Josh Brake
    @version 1.0 7/13/2021
*/
#include <stdio.h>
#include <stm32l432xx.h>
#include "STM32L432KC.h"

////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////

#define MCK_FREQ 100000

// Test Case from Appendix A.1, B
char key[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

char plaintext[16] = {0x32, 0x43, 0xF6, 0xA8, 0x88, 0x5A, 0x30, 0x8D,
                      0x31, 0x31, 0x98, 0xA2, 0xE0, 0x37, 0x07, 0x34};

char ct[16] = {0x39, 0x25, 0x84, 0x1D, 0x02, 0xDC, 0x09, 0xFB,
               0xDC, 0x11, 0x85, 0x97, 0x19, 0x6A, 0x0B, 0x32};

/* 
// Another test case from Appendix C.1

char key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

char plaintext[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                      0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

char ct[16] = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
               0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A};
*/

////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////

void encrypt(char*, char*, char*);
void checkAnswer(char*, char*, char*);

////////////////////////////////////////////////
// Main
////////////////////////////////////////////////

int main(void) {
  char cyphertext[16];

  // Configure flash latency and set clock to run at 84 MHz

  // Enable GPIOA clock
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);


  // Load and done pins
  pinMode(PA5, GPIO_OUTPUT);  // LOAD
  pinMode(PA6, GPIO_INPUT);   // DONE
  
  // debugging LEDs
  pinMode(PA9, GPIO_OUTPUT);
  pinMode(PA10, GPIO_OUTPUT);
  digitalWrite(PA9, 0);
  digitalWrite(PA10, 0);

  // Artificial chip select signal to allow 8-bit CE-based SPI decoding on the logic analyzers.
  pinMode(PA11, GPIO_OUTPUT);
  digitalWrite(PA11, 1);


  // hardware accelerated encryption
  encrypt(key, plaintext, cyphertext);
  checkAnswer(key, plaintext, cyphertext);
}

////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////

void checkAnswer(char * key, char * plaintext, char * cyphertext) {
  // Compare Strings:
  char correct = 1;
  for(int i = 0; i < 16; i++) {
    volatile int k = cyphertext[i] - ct[i];
    
    if(k != 0) {
      correct = 0;
    }
  }

  if(correct) {
    digitalWrite(PA9, 1); // Success!
  } else {
    digitalWrite(PA10, 1);    // Bummer.  Test failed.
  }
  }

void encrypt(char * key, char * plaintext, char * cyphertext) {
  int i;

  // Write LOAD high
  digitalWrite(PA5, 1);

  // Send plaintext
  for(i = 0; i < 16; i++) {
    digitalWrite(PA11, 1); // Arificial CE high
    spiSendReceive(plaintext[i]);
    digitalWrite(PA11, 0); // Arificial CE low
  }

  // Send the key
  for(i = 0; i < 16; i++) {
    digitalWrite(PA11, 1); // Arificial CE high
    spiSendReceive(key[i]);
    digitalWrite(PA11, 0); // Arificial CE low
  }

  while(SPI1->SR & SPI_SR_BSY); // Confirm all SPI transactions are completed
  digitalWrite(PA5, 0); // Write LOAD low

  // Wait for DONE signal to be asserted by FPGA signifying that the data is ready to be read out.
  while(!digitalRead(PA6));

  for(i = 0; i < 16; i++) {
    digitalWrite(PA11, 1); // Arificial CE high
    cyphertext[i] = spiSendReceive(0);  
    digitalWrite(PA11, 0); // Arificial CE low
  }
}
