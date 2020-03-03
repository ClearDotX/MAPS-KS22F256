flexio_uart demo
======================================

Example Description
----------------
This demo is a template project, download and run code in flash, 
demo flexio emulate UART basic transmit and receive function.


The configurations:
 - Debug UART Port Speed: 115200
 - Flexio Port Speed: 115200


Project Content
---------------
  - usecases/flexio_uart/src/main.c		Main program


Hardware and Software environment
---------------------------------
  - This demo runs on the MAPS-KS22 with MAPS-Dock platform
  - This demo runs on the IAR toolchain


How to use
----------
  - Plug in Micro USB, to connect PC and target MAPS-KS22 board
  - Open terminal(such as putty) on PC with 8in1 and 115200bps setings
  - Connect CN7/UART1 on MAPS-Dock board to the PC host
  - Open the project by your toolchain on your pC
  - Rebuild all files and download your image into target flash
  - Run the demo
  - Disconnet CN7/UART1 on MAPS-Dock, Switch to connect CN12/UART2 on MAPS-Dock to the PC host
  - Press any key on the terminal will echo the character enterred.
