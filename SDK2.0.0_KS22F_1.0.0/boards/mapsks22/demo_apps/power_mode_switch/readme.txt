Overview
========
The Power project is a demonstration program that uses the SDK power drivers.
It prints menu through the debug console, user can set the MCU to specific power
mode and set wakeup source follow the debug console tips. The purpose of this demo is
to show how to switch between different power modes and how to wakeup source and
wakeup MCU from low power modes.


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
The power_mode_switch demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
Please DO NOT connect any of the 3 lateral pins in M1 Jumper.
Note: 
     1. The Serial terminal baudrate shall be 9600!!!
     2. Please press "Wakeup Key" longer than common switches to wakeup modes.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 9600 baud rate
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
When runing the demo, the debug console shows the menu to tell how to set MCU
to target power mode.

NOTE: Only input when the demo asks to input, input at other time might make the
debug console overflow and get wrong result.

####################  Power Mode Switch Demo ####################

    Core Clock = 120000000Hz 
    Power mode: RUN

Select the desired operation 

Press  A for enter: RUN      - Normal RUN mode
Press  B for enter: WAIT     - Wait mode
Press  C for enter: STOP     - Stop mode
Press  D for enter: VLPR     - Very Low Power Run mode
Press  E for enter: VLPW     - Very Low Power Wait mode
Press  F for enter: VLPS     - Very Low Power Stop mode
Press  G for enter: LLS/LLS3 - Low Leakage Stop mode
Press  H for enter: VLLS0    - Very Low Leakage Stop 0 mode
Press  I for enter: VLLS1    - Very Low Leakage Stop 1 mode
Press  J for enter: VLLS2    - Very Low Leakage Stop 2 mode
Press  K for enter: VLLS3    - Very Low Leakage Stop 3 mode

Waiting for power mode select..

Customization options
=====================

