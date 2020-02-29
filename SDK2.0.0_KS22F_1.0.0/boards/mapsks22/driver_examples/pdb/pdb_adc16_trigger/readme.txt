Overview
========

The pdb_adc16_trigger example shows how to use the PDB to generate a ADC trigger.

Based on the basic counter, to use the ADC trigger, just to enable the ADC trigger's "milestone" and set the user-defined value for it. 
After the PDB counter is triggered to start, when the counter pass the "milestone", the ADC's Pre-Trigger would be generated and sent 
to the ADC module.

In this example, the ADC16 is configured with hardware trigger and conversion complete interrupt enabled. 
Once it gets the trigger from the PDB, the conversion goes, then the ISR would be executed.


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
The pdb_adc16_trigger example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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


PDB ADC16 Pre-Trigger Example.

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 1
ADC Conversion Interrupt Counter: 1
ADC Conversion Value: 2144

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 2
ADC Conversion Interrupt Counter: 2
ADC Conversion Value: 2145

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 3
ADC Conversion Interrupt Counter: 3
ADC Conversion Value: 2143

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 4
ADC Conversion Interrupt Counter: 4
ADC Conversion Value: 2143

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

Customization options
=====================

