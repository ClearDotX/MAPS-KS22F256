Overview
========

The dac_buffer_interrupt example shows how to use DAC buffer with interrupts.

When the DAC's buffer feature is enabled, user can benefit from the automation of updating DAC output by hardware/software trigger. 
As we know, the DAC converter outputs the value of item pointed by current read pointer. Once the buffer is triggered by software 
or hardware, the buffer's read pointer would move automatically as the work mode is set, like normal (cycle) mode, swing mode, 
one-time-scan mode or FIFO mode.

In this example, it captures the user's type-in operation from terminal and does the software trigger to the buffer. 
The terminal would also display the log that shows the current buffer pointer's position with buffer events.


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
The dac_buffer_interrupt_example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

DAC buffer interrupt Example.

DAC Buffer Information
	  Buffer index max  : 15
	  Buffer watermark  : 14
Press any key in terminal to trigger the buffer ...

Buffer Index  0: ReadPointerTopPositionEvent 	
Buffer Index  1: 
Buffer Index  2: 
Buffer Index  3: 
Buffer Index  4: 
Buffer Index  5: 
Buffer Index  6: 
Buffer Index  7: 
Buffer Index  8: 
Buffer Index  9: 
Buffer Index 10: 
Buffer Index 11: 
Buffer Index 12: 
Buffer Index 13: 
Buffer Index 14: WatermarkEvent 	
Buffer Index 15: ReadPointerBottomPositionEvent 	

Buffer Index  0: ReadPointerTopPositionEvent 	
Buffer Index  1: 
Buffer Index  2: 
Buffer Index  3: 

Customization options
=====================

