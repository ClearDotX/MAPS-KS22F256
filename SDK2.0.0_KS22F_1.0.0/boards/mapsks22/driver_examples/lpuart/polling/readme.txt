Overview
========
The lpuart_polling Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, you can send characters to the console back and they will be printed out onto console
 instantly.


Toolchain supported
===================
- IAR embedded Workbench 7.50.1
- Keil MDK 5.17
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.0.0
- Atollic TrueSTUDIO 5.4.0

Hardware requirements
=====================
- J-Link ARM
- Mini USB cable
- USB-to-Serial connector
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The lpuart_polling example is configured to use LPUART0 with PTE4 and PTE5 pins. 
To observe the execution log on serial port, the USB-to-Serial connector should be connected to CN7 port on
MAPS-DOCK board and open the lpuart COM port in PC.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for 
   KS22F25612 (document SDKKS22F256GSUG).

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

Lpuart polling example
Board will send back received characters
12 34 lskdk liwoeru helool 

Customization options
=====================

