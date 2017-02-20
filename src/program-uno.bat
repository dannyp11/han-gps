@echo off
avrdude -p atmega328p -c arduino -P COM3 -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -U flash:w:han-uno.hex
