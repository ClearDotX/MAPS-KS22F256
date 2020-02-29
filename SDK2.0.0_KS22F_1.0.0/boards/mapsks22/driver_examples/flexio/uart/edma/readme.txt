Overview
========
The flexio_uart_interrupt example shows how to use flexio uart driver in edma way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters 
that PC send to the board.


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
- USB-to-Serial converter

Board settings
==============
The flexio_uart_edma example is configured to use FLEXIO D0 and D1 with PTC12 and PTC13 pins.
PTC12: FLEXIO_UART_TX_PIN, CN9D-84
PTC13: FLEXIO_UART_RX_PIN, CN9D-85
To observe the execution log on serial port, PTC12 and PTC13 should connect to the USB-to-Serial connector plug on PC, 
open the related COM port on PC to monitor the execution log.
PTC12(Pin-84)  <--> USB-to-Serial(RX end)
PTC13(Pin-85)  <--> USB-to-Serial(TX end)
MAPS-Board_GND <--> USB-to-Serial(GND end)

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

Flexio uart edma example
Board receives 8 characters then sends them out
Now please input:

Customization options
=====================

