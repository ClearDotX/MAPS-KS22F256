Overview
========
The flexio_spi_master_interrupt_dspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a dspi slave .


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
The flexio_spi_master_interrupt_dspi_slave example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 part jumpers connected
The flexio_spi_master_interrupt_dspi_slave example is configured to use FLEXIO D0, D1, D2 and D3 with PTC12, PTC13, PTC14 and PTC15 to simulate spi master and communicate with a on board dspi slave, with pin connections below:
PTC12: FLEXIO_SPI_SOUT_PIN, CN9D-84(master) <--> CN9D-100(slave)
PTC13: FLEXIO_SPI_SIN_PIN,  CN9D-85(master) <--> CN9D-99(slave)
PTC14: FLEXIO_SPI_CLK_PIN,  CN9D-86(master) <--> CN9D-98(slave)
PTC15: FLEXIO_SPI_PCS_PIN,  CN9D-87(master) <--> CN9D-97(slave)

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
Run the slave first, then run the master to start transfer.
When the example runs successfully, you can see the similar information from the terminal as below.

FLEXIO Master - DSPI Slave interrupt example start.
This example use one flexio spi as master and one dspi instance as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
FLEXI_SPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is DSPI slave call back .

FLEXIO SPI master <-> DSPI slave transfer all data matched!


Customization options
=====================

