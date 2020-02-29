Overview
========

The adc16_polling example shows the simplest way to use ADC16 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC16's sample input.
When running the project, typing any key into debug console would trigger the conversion. The execution would check the conversion 
completed flag in loop until the flag is asserted, which means the conversion is completed. Then read the conversion result value and 
print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change the 
configuration structure's setting in application to fit the special requirement.

The auto-calibration is not essential but strongly recommended. It can help to adjust the converter itself and improve the 
ADC16's performance.


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
The adc16_polling_example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

ADC16 polling Example.
ADC16_DoAutoCalibration() Done.
Press any key to get user channel's ADC value ...
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
ADC Value: 4095
......

Customization options
=====================

