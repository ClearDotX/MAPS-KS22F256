Overview
========
The flexcan_interrupt exmalpe shows how to use FlexCAN dirver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receive the message, print 
the message content to terminal and echo back the message. Endpoint A will increase the received
message and waiting for the next transmission of the user initiated.


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
The flexcan_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
To enable flexcan communication on two boards, connect same port on each board, that is:
- Board1_CN7_Port1 <--> Board2_CN7_Port1
- Board1_CN7_Port2 <--> Board2_CN7_Port2

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

1. Connect the 2 board together through CAN Bus using twisted-pair;
2. Open terminals of these 2 boards and select the Role of specified board. Node A is used for
   trigger transmission. Node B listens for the CAN Bus continuously and echo back the received
   CAN Message to Node A.
3. Now, you can press the Space key in Node A's terminal and Observate if the data is transfered
   success.

Customization options
=====================

