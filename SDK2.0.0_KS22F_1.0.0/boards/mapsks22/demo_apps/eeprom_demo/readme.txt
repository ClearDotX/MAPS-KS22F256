Overview
========
The eeprom flash demo is a demonstration program that uses the SDK software.  It uses lpi2c driver to write 
and read back data from the eeprom.  

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
The eeprom demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
Connections on MAPS-KS22F256 board need to be setup as follows:
- JP13 Disonnected
- JP14 Disonnected

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
When the demo runs successfully, the following message will be seen on the OpenSDA terminal:
EEPROM Demo
This demo will write data into on board EEPROM and then read it back.
Write data at 0x0 is 0xff, read back data is 0xff
Write data at 0x1 is 0xfe, read back data is 0xfe
Write data at 0x2 is 0xfd, read back data is 0xfd
Write data at 0x3 is 0xfc, read back data is 0xfc
Write data at 0x4 is 0xfb, read back data is 0xfb
Write data at 0x5 is 0xfa, read back data is 0xfa
Write data at 0x6 is 0xf9, read back data is 0xf9
Write data at 0x7 is 0xf8, read back data is 0xf8
    .............
End of the demo.

Customization options
=====================

