Overview
========
The flexcan_loopback exmalpe shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The exmaple will config one FlexCAN Message
Buffer to Rx Message Buffer and the other FlexCAN Message Buffer to Tx Message Buffer with same ID.
After that, the exmaple will send a CAN Message from the Tx Message Buffer to the Rx Message Buffer
throuth internal loopback interconnector and print out the Message payload to terminal.


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
The flexcan_loopback example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

1. Connect the board to PC and open Terminal;
2. Press the Reset Button of the board;
3. You can see following content in Terminal:

    ==FlexCAN loopback example -- Start.==

    Send message from MB9 to MB8
    tx word0 = 0x11223344
    tx word1 = 0x55667788

    Receved message from MB8
    rx word0 = 0x11223344
    rx word1 = 0x55667788

    ==FlexCAN loopback example -- Finish.==

From the screen display, you can see that the tx data and rx data are same.

Customization options
=====================

