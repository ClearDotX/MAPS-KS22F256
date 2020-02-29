Overview
========
This example demonstrates the SDK Peripheral drivers working with different methods.

- Using non-blocking method

Transfer data between the board and the smart card Zeitcontrol ZC7.5 RevD. The example transfers
using both T0 and T1 modes.


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
- Smart card of Zeitcontrol ZC7.5 RevD
- Smart card Dock

Board settings
==============
The smartcard example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
Before running the demo, connect your smartcard dock to CN8 connector on MAPS-KS22F256 board.

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

~~~~~~~~~~~~~~~~~~~~~

***** SMARTCARD Driver Send Receive functionality example *****

Card inserted.
Deactivating card...Done!
Resetting/Activating card...Done!
Send receive functionality example in T=0 mode started!

Smartcard EEPROM Start Address = 0xC00000
Smartcard EEPROM Size = 32 KBytes

Deactivating card...Done!
Resetting/Activating card...Done!

Send receive functionality example in T=1 mode started!

LRC received correctly
Echo data received correctly!
Command status OK

Send receive functionality example finished!
~~~~~~~~~~~~~~~~~~~~~


Customization options
=====================


