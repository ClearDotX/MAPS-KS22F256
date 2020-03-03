LPI2C slave work in VLPS mode with async DMA
============================================

Example Description
-------------------
This example describes how to use HAL drivers to implement LPI2C
slave transmit and receive in the VLPS (very low power STOP mode) with async DMA.
The LPI2C module is designed to have ability to work under low power
module like STOP, VLPW and VLPS. It can use DMA to transmit the
data from or to application user buffer without CPU interaction.

The slaver act as a sensor hub, provide a internal buffer (16bytes)
to store data. Master can use Master-Receive to read the buffer
by Single-byte or Multiple-byte read operations, the Multiple-byte read
would make slave send the data in the buffer in loop until a STOP.
The master can also use Master-Transmit to write data into the slave
internal buffer by Single-byte or Multiple-byte write operations.

This example has two projects: master and slave.
They run on two differnet boards, with LPI2C0 SDA/SCL/GND pin connected (CN5 3,4,2).

NOTE: Cause the LPI2C IP slave would transmit a extra data when using TDF
for DMA or SW method Tx, before STOP can be detect. So slave-transmit
would send a extra data onto the SDA bus, and DMA offset would change.


The slave configurations:
 - System run under BLPI clock mode with 4Mhz IRC as clock source
 - EDMA & DMAMUX
   - Channel0 is used for LPI2C Transmit
   - Channel1 is used for LPI2C Receive
   - Channel0 transmit bytes to MTDR with command and data
   - Channel1 receive bytes from MRDR with data
   - Work in async mode
   - Disable interrupt after major loop done
   - Not disable DMA request after major loop done
   - 1Byte per Minor loop
   - 16Bytes per Major loop
 - LPI2C
   - configured to work in DOZE mode with DMA request enabled.
   - 400Kbps baud rate, with clock source from IRC
   - Work in slave mode

The master configurations:
 - System run under BLPI clock mode with 4Mhz IRC as clock source
 - LPI2C
   - 400Kbps baud rate, with clock source from IRC
   - Work in master mode


Project Content
---------------
  - examples/mapsks22f256/demo_apps/lpi2c_slave_vlps/src/master.c    Master program
  - examples/mapsks22f256/demo_apps/lpi2c_slave_vlps/src/slave.c     Slave program


Hardware and Software environment
---------------------------------
  - This demo runs on the MAPS-KS22F256 platform
  - This demo runs on the IAR toolchain


How to use
----------
  - Open the project by your toolchain
  - Rebuild all files and download master and slave image into two boards
  - Open terminal on PC with 8in1 and 115200bps setings on master side
  - Run the demo
