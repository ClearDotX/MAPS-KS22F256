flexio_i2s demo
======================================

Example Description
----------------
This demo demonstartes how to use FlexIO emulate I2S master tx to 
communicate with WM8960 Codec on MAPS-Dock board.


The configurations:
 - Debug UART Port Speed: 115200


Project Content
---------------
  - usecases/flexio_i2s/src/main.c		Main program
  - usecases/flexio_i2s/src/wm8960.c		codec driver
  - usecases/flexio_i2s/src/music.h		music wave file



Hardware and Software environment
---------------------------------
  - This demo runs on the MAPS-KS22 with MAPS-Dock platform
MAPS-KS22 pin assignment are listed below.
    - I2S0_MCK Pin:       JP5 pin3
    - I2S0_TX_BCLK Pin:   JP6 pin3
    - I2S0_TXD0 Pin:      JP7 pin3
    - I2S_TX_FS Pin:      JP8 pin3
    - TPM1_CH1 simulate I2S_MCLK Pin:   JP5 pin1
    - FlexIO I2S TX BCLK Pin:           JP6 pin1
    - FlexIO I2S TXD Pin:               JP7 pin1
    - FlexIO I2S TX FS Pin:             JP8 pin1
You need to make below jumper connections.
    JP5 pin1-pin2     JP6 pin1-2
    JP7 pin1-pin2     JP8 pin1-2
  - This demo runs on the IAR toolchain


How to use
----------
  - Plug in Micro USB, to connect PC and target MAPS-KS22 board
  - Open terminal(such as putty) on PC with 8in1 and 115200bps setings
  - Make above jumper connections
  - Open the project by your toolchain on your pC
  - Rebuild all files and download your image into target flash
  - Run the demo
  - To hear the audio playback, connect a set of headphones to the headphone output on the MAPS-DOCK
board CN10.
