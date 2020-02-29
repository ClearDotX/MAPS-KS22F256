Overview
========
This RTC demo application demonstrates the important features of the RTC Module by using the RTC
Peripheral Driver.

It supports these features:
- Calendar
  + Get the current date time with Year, Month, Day, Hour, Minute and Second.
  + Set the current date time with Year, Month, Day, Hour, Minute and Second.
- Alarm
  + Set the alarm based on the current time.
  + Application prints a notification when the alarm expires.
- Seconds interrupt
  + Use second interrupt function to display a digital time blink every second.


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
The rtc_func demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
When the demo run successfully, you will see information in serial terminal log:

RTC Demo running...

Please choose the sub demo to run:
1) Get current date time.
2) Set current date time.
3) Alarm trigger show.
4) Second interrupt show (demo for 20s).

Select:1
Current datetime: 2015-11-11 11:11:13

Please choose the sub demo to run:
1) Get current date time.
2) Set current date time.
3) Alarm trigger show.
4) Second interrupt show (demo for 20s).

Select:3
Input the alarm seconds from now on (1s~9s):3
Triggered Alarm: 11:11:22

Please choose the sub demo to run:
1) Get current date time.
2) Set current date time.
3) Alarm trigger show.
4) Second interrupt show (demo for 20s).

Select:

Customization options
=====================

