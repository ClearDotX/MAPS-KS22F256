Overview
========
The GPIO Example project is a demonstration program that uses the KSDK software to manipulate the general-purpose outputs.
The example is supported by the set, clear, and toggle write-only registers for each port output data register. The example uses the software button to
control/toggle the LED.


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
The gpio_input_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
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
These instructions are displayed/shown on the terminal window:
GPIO Driver example
Press SW to turn on/off a LED
If you press the SW, the LED will be toggled, and "SW is pressed" is shown on the terminal window.

Customization options
=====================


