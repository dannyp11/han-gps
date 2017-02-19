#!/usr/bin/env bash

find ../ChibiOS-src -type f -iname "*.[chS]" | grep -v MSP | grep -v TIVA | grep -v STM32 | grep -v SPC5 | grep -v LPC | grep -v KINETIS | grep -v test | xargs ctags -a
