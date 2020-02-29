Overview
========
The LPUART example for FreeRTOS demonstrates the possibility to use the LPUART driver in the RTOS.
The example uses single instance of LPUART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on LPUART.

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
The freertos_lpuart example is configured to use LPUART0 with PTE4 and PTE5 pins. 
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
You will see the welcome string printed out on the console.
You can send characters to the console back and they will be printed out onto console in a group of 4 characters.

Customization options
=====================

