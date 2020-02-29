Overview
========

This application demonstrates how to configure the DAC and set the output on the DAC using software.
It also demonstrates how to configure the ADC in 'Blocking Mode' and read ADC values.


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
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The dac_adc_demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected;

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

This example shows how to run the demo:

DAC ADC Demo!
See Kinetis SDK Demo Applications User's Guide document,
Chapter DAC ADC demo, for pins configuration information.
Press any key to start demo.

Demo begin...
ADC16_DoAutoCalibration() Done.

The user is prompted to enter a voltage to output on the DAC:

Select DAC output level:
        1. 1.0 V
        2. 1.5 V
        3. 2.0 V
        4. 2.5 V
        5. 3.0 V

After entering a valid input, the ADC captures the voltage set by the DAC
and displays the result in the terminal:

Select DAC output level:
        1. 1.0 V
        2. 1.5 V
        3. 2.0 V
        4. 2.5 V
        5. 3.0 V
->3
ADC Value: 2471
ADC Voltage: 1.99
What next?:
        1. Test another DAC output value.
        2. Terminate demo.

At this point, the user can test another DAC output value or terminate the demo.

This configuration exhibits up to 2% error when reading back voltage.

Customization options
=====================


