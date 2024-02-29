# ESP8266 Webserver Code

## Overview

This code hosts a webserver displaying webpages sent over UART.

In more detail:

1) The webserver starts a 125000 baud serial connection over the hardware UART (for debug)
2) The webserver connects to a given network or creates its own.
3) The webserver waits for a request from the client. When it receives one, it transmits /REQ:<path>\n to the MCU. 

Step 3 is repeated while the program runs

## Connecting to ESP

1) Connect to the ESP's WiFi network
2) Go to http://192.168.4.1/ after your computer is assigned an IP address by the ESP. 

## Programming the ESP

To program, need to reset while the programming pin is held down.
The pins to program are also broken out on the through-hole headers on the Mikroe Click board.

Instructions [here](https://learn.edwinrobotics.com/getting-started-with-esp-wroom-02/)

- Press both the RESET button and PROGRAM Button together.
- Leave the RESET button alone and keep Pressing the PROGRAM button for 1 more second.
- Leave the PROGRAM Button, now you are in UART Download mode.

## Acknowledgements
This code was originally developed by Erik Meike and Kaveh Pezeshki and Christopher Ferrarin.
It was updated and modified by Josh Brake in the Fall of 2019.
