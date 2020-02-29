Overview
========
The RTC project is a simple demonstration program of the SDK RTC driver. It sets up the RTC
hardware block to trigger an alarm after a user specified time period. The test will set the current
date and time to a predefined value. The alarm will be set with reference to this predefined date
and time.


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
The rtc example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

RTC example: set up time to wake up an alarm
Current datetime: 2014-12-25 19:00:00
Please input the number of second to wait for alarm 
The second must be positive value
Alarm will occur at: 2014-12-25 19:00:08

 Alarm occurs !!!! Current datetime: 2014-12-25 19:00:09
Please input the number of second to wait for alarm 
The second must be positive value

Customization options
=====================

