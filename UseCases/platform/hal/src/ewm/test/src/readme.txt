Test Scope:

This unit test covers most of WDOG hal interface and verify its operation.

NOTES:

Make sure project is rebuild before testing.

Test Setup:

1. BAUDRATE: 115200.
2. UART PORT:
   2.1 k22: UART port on CPU board.
   2.1 k64: UART port on CPU board.
   2.1 k70: UART port on CPU board.

Output:
1.When test the TRUE function, the output message is below: 
EWM false test successful!
2.When test the FALSE function, the output message is below:
EWM true test successful!
