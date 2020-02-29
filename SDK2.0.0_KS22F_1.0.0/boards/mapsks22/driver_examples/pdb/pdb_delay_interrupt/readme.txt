Overview
========

The pdb_delay_interrupt example show how to use the PDB as a general programmable interrupt timer.

The PDB is triggered by software, and other external triggers are generated from PDB in this project, 
so that user can see just a general counter is working with interrupt.


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
The pdb_delay_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
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

PDB Delay Interrupt Example.

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 1

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 2

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 3

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 4

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 5

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 6

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 7

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 8

Type any key into terminal to trigger the PDB counter ...

Customization options
=====================

