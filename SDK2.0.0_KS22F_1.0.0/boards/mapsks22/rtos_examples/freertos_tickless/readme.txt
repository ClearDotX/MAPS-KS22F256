Overview
========
This document explains the freertos_tickless example. It shows the CPU enter at sleep mode and then 
it is waked up by expired time delay that using LPTMR module or occur any a hardware interrupt.


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
The freertos_tickless example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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
After the board is flashed the Tera Term will start periodically printing number tick count when the CPU 
is running.

When the CPU is running and get tick count number, press SW3 button on board to wake up CPU then Tera Term 
will printing “CPU waked up by EXT interrupt” message.

Example output:

Tickless Demo example

Tick count :
1
5001
CPU waked up by EXT interrupt
10001
CPU waked up by EXT interrupt
15001
CPU waked up by EXT interrupt
CPU waked up by EXT interrupt
20001
25001
30001

Explanation of the example
The example application will get tick count number after every period time delay vTaskDelay(time)
When this function called the CPU enter to sleep mode a period of time that called by vTaskDelay 
function.

While the CPU is sleeping, if press SW3 button once or several time on board then CPU waked up 
by external interrupt and continuous enter sleep mode after complete interrupt service.
The period of time delay is not change after occur external interrupt.

Customization options
=====================

