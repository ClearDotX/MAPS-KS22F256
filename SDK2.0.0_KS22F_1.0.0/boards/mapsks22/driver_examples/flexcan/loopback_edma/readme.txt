Overview
========
The flexcan_loopback_edma exmalpe shows how to use the EDMA version transactional driver to receive
CAN Message from Rx FIFO:

To demonstrates this example, only one board is needed. The exmaple will config one FlexCAN Message
Buffer to Tx Message Buffer and also setup Rx FIFO. After that, the exmaple will send 4 CAN Messages
from Tx Message Buffer to Rx FIFO throuth internal loopback interconnector and read them out using
EDMA version FlexCAN transactional driver. The Sent and received message will be print out to terminal
at last.


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
The flexcan_loopback_edma example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
3. From the screen display, you can observe that if the tx data and rx data are same.

Customization options
=====================

