Overview
========

The CMP polling Example shows the simplest way to use CMP driver and help user with a quick start.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the CMP's positive channel input. On the negative side, the internal 6-bit DAC is used to generate the fixed voltage about half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change between logic one and zero when the user's voltage crosses the internal DAC's value. The endless loop in main() function would detect the logic value of comparator's output, and change the LED. The LED would be turned on when the compare output is logic one, or turned off when zero.


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
The cmp_polling_example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
To observe the example result, please connect on & off Pin-78 to GND pad in turn on MAPS-KS22F256 board.

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

CMP polling Example.

Customization options
=====================

