# How this demo works

## Software Serial

### Hardware and Behavior

If using UNO: PD2 is RX, PD3 is TX
If using Project Board: PD2 is RX, PC0 is TX

Baud is set to 4800. Typing in software serial terminal will echo.

### Relavent Code

Sample code is in `main.c`. Also, read `debug.h` for some debugging macros.

## USART1

### Hardware and Behavior

Just the normal serial port. Does exactly the same thing as the software serial port.

### Relavent Code

Sample code is in `monitor.c`. `debug.h` is relavent, too.