# Starter Code for Lab 7
The starter code for Lab 7 contains code for both the FPGA and MCU.
The MCU code is complete as written and sends data over an SPI link to the FPGA which performs the AES cipher.
You should make sure that your design on the FPGA is connected to the correct MCU pins to interface the two devices based on the pins specified in the MCU source code.

Your job is to write the code to implement the cipher on the FPGA.
You are provided with three testbenches to assist in your testing.
1. `aes_spi_tb.sv` simulates the entire AES system including shifting in the data over SPI.
2. `aes_core_tb.sv` simulates the AES core module by itself without the additional complication of getting the data in and out of the system via the shift registers and SPI. You should make sure your design passes this testbench before proceeding to test with `aes_spi_tb.sv`.
3. `aes_sbox_tb.sv` is a simple testbench to illustrate the single clock cycle delay that is inherent in using a synchronous embedded block RAM. This single cycle of delay must be considered in your FSM design in order for it to work properly.

`sbox.txt` is a text file containing the data for the sbox substitution and is used to configured the RAMs on the FPGA.