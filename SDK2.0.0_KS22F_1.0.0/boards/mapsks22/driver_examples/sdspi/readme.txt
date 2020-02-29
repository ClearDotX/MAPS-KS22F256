Overview
========

The SDSPI example shows how to use SDSPI driver to initializes SD card and read/write data blocks 
of SD card. It calls read/write APIs of the driver to read/write single and multiple data blocks
of SD card forever until encounters some error to result in the read data content is different the 
original data content written to the card.

The example only uses the polling transaction API of SPI driver to send/receive data 
over SPI bus. More detail transaction API of SPI driver can refer to the SPI driver and SPI example.


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
The sdspi example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
The SDSPI example is configured to use SPI1 as master to communicate with SDCARD on the MAPS-DOCK 
board as slave. 
SPI1 on the MAPS-KS22F256 board is configured to use PTC3 as PCS pin, PTC5 as SCK 
pin, PTD3 as MISO pin, PTD2 as MOSI pin. 
To make SDCARD on the MAPS-DOCK board work, the jumpers of JP8, JP9 and JP13 on the MAPS-DOCK board 
should be removed.

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
When the example runs successfully, following information can be seen on the OpenSDA terminal.
"SD card over SPI example start."
"Please insert the SD card."
"Detected SD card inserted."
"Read/write SD card continuously until encounters error."

Customization options
=====================
1. <FIXME>The USE_STDIO_FUNCTIONS definition determines whether the example uses standard I/O
   functions, such as printf. If it is not defined, then the example accesses the UART driver directly.

