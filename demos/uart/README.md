# UART Demo

This folder contains partially completed code snippets to configure the UART on the MCU.

You need to refer to the reference manual to determine how to configure the UART to operate with the following settings:
* 8 data bits
* No parity bit
* 1 stop bit
* Operate at 9600 baud (9.6 Kbps)

The code is configured to use the high-speed internal (HSI) oscillator running at 16 MHz.
Use 16 MHz as the value to configure the baud rate.

After configuring the UART, use it to print a string to the serial monitor!
