flexio_spi demo
======================================

Example Description
----------------
This demo describes how to use FlexIO module to emulate SPI master and 
slave loopback transfer test with DMA mode.


The configurations:
 - Debug UART Port Speed: 115200


Project Content
---------------
  - usecases/flexio_spi/src/main_loopback.c		Main program


Hardware and Software environment
---------------------------------
  - This demo runs on one MAPS-KS22 with MAPS-Dock platform
MAPS-KS22 pin assignment are listed below.
    - FlexIO SPI master TX Pin:   CN9D pin84
    - FlexIO SPI master RX Pin:   CN9D pin85
    - FlexIO SPI master SCK Pin:  CN9D pin86
    - FlexIO SPI master CS Pin:   CN9D pin87
    - FlexIO SPI slave  TX Pin:   CN9D pin90
    - FlexIO SPI slave  RX Pin:   CN9D pin91
    - FlexIO SPI slave  SCK Pin:  CN9D pin93
    - FlexIO SPI slave  CS Pin:   CN9D pin94
You need to make the connections between the master and slave by using 4 external wires.
    SPI master TX - SPI slave RX,
    SPI master RX - SPI Slave TX,
    SPI master SCK - SPI slave SCK,
    SPI master CS - SPI slave CS,
  - This demo runs on the IAR toolchain


How to use
----------
  - Plug in Micro USB, to connect PC and target MAPS-KS22 board
  - Open terminal(such as putty) on PC with 8in1 and 115200bps setings
  - Connect CN7/UART1 on MAPS-Dock board to the PC host
  - Open the project by your toolchain on your pC
  - Make the connections between the master and slave by using 4 external wires.
  - Rebuild all files and download your image into target flash
  - Run the demo
  - After the data transfers, the results are printed on the master terminals.

