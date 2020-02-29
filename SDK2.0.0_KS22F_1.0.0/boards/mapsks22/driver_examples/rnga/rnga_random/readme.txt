Overview
========
The RNGA is a digital integrated circuit capable of generating the 32-bit random numbers.
The RNGA Example project is a demonstration program that uses the KSDK software to generate random numbers and prints them to the terminal.


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
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The rnga_random example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected

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
When the demo runs successfully, from the terminal you can see:

RNGA Peripheral Driver Example
Generate 10 random numbers:
Random[0] = 0xE1554295
Random[1] = 0x827AD456
Random[2] = 0x9A1CBE1E
Random[3] = 0x4354CB53
Random[4] = 0xFE3B2494
Random[5] = 0xEDAB3F7D
Random[6] = 0x9AB91722
Random[7] = 0x4F54D999
Random[8] = 0x492414D1
Random[9] = 0x84611992

 Press any key to continue...

Customization options
=====================

