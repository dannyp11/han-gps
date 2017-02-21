#!/usr/bin/env bash

#find ../ChibiOS-src -type f -iname "*.[chS]" | grep -v MSP | grep -v TIVA | grep -v STM32 | grep -v SPC5 | grep -v LPC | grep -v KINETIS | grep -v test | xargs ctags -a

DIRS=`echo | avr-gcc -E -Wp,-v - 2>&1 | egrep '^ '`
DIRS+=`printf "\n."`
rm -f tags
for d in $DIRS ; do
	find $d -type f -iname "*.[chS]" | grep -v CMSIS | grep -v e200 | grep -v ARM | grep -v SIMIA32 | grep -v NRF5 | grep -v MSP | grep -v TIVA | grep -v STM32 | grep -v SPC5 | grep -v LPC | grep -v KINETIS | grep -v test | xargs ctags -a
done
