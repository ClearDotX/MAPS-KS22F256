I2S audio bridge use case
=========================

Example Description
-------------------
This use case shows how to use two I2S at the same time by DMA.
One I2S instance is used to do audio recording from WM8960 codec
with the format of 24bit, left-justified, 8KHz sample rate.
System routes the PCM stream directly to the other I2S instance,
which can be connected to another audio codec with configurations
of 16bit, left-justified, 8KHz sample rate. This means the audio
bridge converts the 24bit PCM to 16bit PCM.


The I2S configurations:
 - I2S0
   - Rx with 8KHz sample rate, left-justified, 24bits.
 - I2S1
   - Tx with 8KHz sample rate, left-justified, 16bits.

The DMA configuraions:
 - DMAMUX
   - Channel0 is used for I2S0 Receive
   - Channel1 is used for I2S1 Transmit
 - DMA for I2S Tx
   - Channel0 receive bytes from RDR
   - Work in async mode
   - Disable interrupt after major loop done
   - Not disable DMA request after major loop done
   - 4bytes x FIFO watermark per Minor loop
   - buffer size per Major loop
 - DMA for I2S Rx
   - Channel1 receive bytes from TDR
   - Work in async mode
   - Disable interrupt after major loop done
   - Not disable DMA request after major loop done
   - 4bytes x (FIFO size - watermark) per Minor loop
   - buffer size per Major loop


Project Content
---------------
  - examples/mapsks22f256/demo_apps/i2s_audio_bridge/src/main.c		Main program
  - examples/mapsks22f256/demo_apps/i2s_audio_bridge/src/wm8960.c       WM8960 codec driver


Hardware and Software environment
---------------------------------
  - This demo runs on the MAPS-KS22F256 platform
  - This demo runs on the IAR toolchain


How to use
----------
  - Plug in Micro USB, to connect PC and target board
  - Open the project by your toolchain on your pC
  - Rebuild all files and download your image into target flash
  - Run the demo
