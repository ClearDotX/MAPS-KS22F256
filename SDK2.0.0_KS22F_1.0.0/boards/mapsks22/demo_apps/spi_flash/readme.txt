Overview
========
The spi flash demo is a demonstration program that uses the SDK software.  It uses spi driver to erase,
program and read the flash chip.  


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
The spi_flash demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
When the example runs successfully, you can see the similar information from the terminal as below.


***SPI Flash Demo***

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5): 1

SF: Chip successfully erased!
Erase OK!

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5): 2

Input the sector address(HEX), 0x00
Input the sector length:4096

SF: Sector(s) successfully erased!
Erase OK!

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5): 3

Input the block address(HEX), 0x00
1. 32K block size
2. 64K block size
Select the block size:1

SF: Block successfully erased!
Erase OK!

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5): 4

Input the program address(HEX), 0x00

SF: Sector(s) successfully erased!

SF: program success!
Program and verify done!

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5): 5

Input the read address(HEX), 0x05
0x5 = 0x5a

Customization options
=====================

