Overview
========
The ADC Low Power Demo project is a demonstration program that uses the KSDK software. The
microcontroller is set to a very low power stop (VLPS) mode, and every 500 ms an interrupt wakes up
the ADC module and takes the current temperature of the microcontroller. While the temperature
remains within boundaries, both LEDs are on. If the temperature is higher or lower than average,
a led comes off. This demo provides an example to show how ADC works during a VLPS mode and a simple
debugging.


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
The adc_low_power demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
When runing the demo, the debug console shows some messages and some instructions should be
displayed on the terminal to tell how to use

NOTE: Only input when the demo required to input, input at other time might make the
debug console overflow and get wrong result.

 ADC LOW POWER DEMO

 The Low Power ADC project is designed to work with the Tower System or in a stand alone setting


1. Set your target board in a place where the temperature is constant.

2. Wait until two Led light turns on.

3. Increment or decrement the temperature to see the changes.

Wait two led on...

Enter any character to begin...

 ---> OK! Main process is running...

Customization options
=====================

