LPI2C master work in VLPS mode with async DMA
============================================

Example Description
-------------------
This example describes how to use HAL drivers to implement LPI2C
master transmit and receive in the VLPS (very low power STOP mode) with async DMA.
The LPI2C module is designed to have ability to work under low power
module like STOP, VLPW and VLPS. It can use DMA to transmit the
data from or to application user buffer without CPU interaction.

The master would read the data in MAPS-Dock I2C EEPROM AT24C02
with 64bytes per second. The system would enter VLPS mode by default,
and a Low Power timer is configured to wake up system and enable
the DMA trigger for LPI2C Tx/Rx every 1s. Then system go back to VLPS
mode with the DMA Tx started. LPI2C send START and the I2C device address
of AT24C02 on the bus with READ request. Then DMA Rx started to
receive data from LPI2C receive data register to fill 64bytes buffer.
Then print the 64bytes out on the LPUART0 console.


The configurations:
 - System run under BLPI clock mode with 4Mhz IRC as clock source
 - EDMA & DMAMUX
   - Channel0 is used for LPI2C Transmit
   - Channel1 is used for LPI2C Receive
   - Channel0 transmit bytes to MTDR with command and data
   - Channel1 receive bytes from MRDR with data
   - Work in async mode
   - Disable interrupt after major loop done for Channel0
   - Enable interrupt after major loop done for Channel1
   - Disable DMA request after major loop done
   - 1Byte per Minor loop
   - 64Bytes per Major loop
 - LPI2C
   - 100Kbps baud rate, with clock source from IRC
   - Work in master mode


Project Content
---------------
  - examples/mapsks22f256/demo_apps/lpi2c_master_vlps/src/main.c    Master program


Hardware and Software environment
---------------------------------
  - This demo runs on the MAPS-KS22F256 platform with MAPS-Dock connected
  - This demo runs on the IAR toolchain


How to use
----------
  - Open the project by your toolchain
  - Rebuild all files and download image into board
  - Open terminal on PC with 8in1 and 115200bps setings on master side
  - Run the demo
