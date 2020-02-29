Overview
========
The adc potentiometer demo is a demonstration program that uses the SDK software.  It uses adc driver to 
convert the potentiometer input analog value to a digital one.


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
The adc_potentiometer demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
After running the demo, change the potentiometer's value, you'll see ADC16 output value changing.
These instructions are displayed/shown on the terminal window:
ADC16_DoAutoCalibration() Done.

The ADC16 output value is 1292.

The ADC16 output value is 1292.

The ADC16 output value is 1291.

The ADC16 output value is 1525.

The ADC16 output value is 1511.

The ADC16 output value is 1329.

The ADC16 output value is 1179.

The ADC16 output value is 1180.

The ADC16 output value is 1180.
......

Customization options
=====================

