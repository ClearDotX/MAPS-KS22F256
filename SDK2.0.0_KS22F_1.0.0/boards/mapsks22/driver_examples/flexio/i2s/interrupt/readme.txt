Overview
========
The flexio_i2s_interrupt example shows how to use flexio_i2s driver with interrupt:

In this example, flexio acts as I2S module to playbacks the audio data stored in flash using interrupt.


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
The flexio_i2s_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
The flexio i2s example needs to connect to WM8960 codec, in MAPS-KS22F256 board (Yellow board), the jumper setting
should be:
- JP5 : 2-3
- JP6 : 1-2
- JP7 : 1-2
- JP8 : 1-2
- JP13 Connected
- JP14 Connected
On MAPS-DOCK:
- JP1 all on

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
And you can hear a piece of sound which is a sin wave.

FLEXIO_I2S interrupt example started!

FLEXIO_I2S interrupt example finished!

Customization options
=====================
Notice: Flexio can not have Mater clock connected to WM8960 codec in MAPS board sue to hardware limit. As WM8960 need  a master clock to work,this example uses SAI master clock to enable WM8960. As the clock from flexio is not accurate and have some mismatch with SAI master clock, so the output voice may have some noise.

