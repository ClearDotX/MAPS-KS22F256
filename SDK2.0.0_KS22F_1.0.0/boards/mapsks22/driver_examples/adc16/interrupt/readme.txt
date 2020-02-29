Overview
========

The adc16_interrupt example shows how to use interrupt with ADC16 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC16's sample input.
When running the project, typing any key into debug console would trigger the conversion. ADC interrupt would be asserted once the 
conversion is completed. In ADC ISR, the conversion completed flag would be cleared by reading the conversion result value. 
Also, the conversion result value is stored, and the ISR counter is increased. These information would be printed when the execution 
return to the main loop.

The point is that the ADC16 interrupt configuration is set when configuring the ADC16's conversion channel. When in software trigger 
mode, the conversion would be launched by the operation of configuring channel, just like writing a conversion command. So if user 
wants to generate the interrupt every time the conversion is completed, the channel's configuration with enabling interrupt setting 
would be used for each conversion.


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
The adc16_interrupt_example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

ADC16 interrupt Example.
ADC16_DoAutoCalibration() Done.
Press any key to get user channel's ADC value ...
ADC Value: 4095
ADC Interrupt Count: 1
ADC Value: 4095
ADC Interrupt Count: 2
ADC Value: 4095
ADC Interrupt Count: 3
ADC Value: 4095
ADC Interrupt Count: 4
ADC Value: 4095
ADC Interrupt Count: 5
......

Customization options
=====================

