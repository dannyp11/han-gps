@echo off
avrdude -p atmega328p -c usbtiny -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -U flash:w:han-cstm.hex
