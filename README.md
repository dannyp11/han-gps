# How to build
Use a Unix-like system, have gnumake, avr-gcc and avrdude installed. Windows might need some love,
but command line would probably work.

Depending on the board, modify `BOARD_CHOSEN` in `src/Makefile`.

    $ cd src/
    $ make
    $ make program # Flash the device

# Roadmap

- [ ] Create a roadmap
