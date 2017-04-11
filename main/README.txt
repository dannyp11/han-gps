# run: 
	make ; make flash
	
# code structure:
	include/*.h : 		all project include files, doesn't separate sw, hw, etc.
	src/hardware/*.c:	driver for components like lcd, led, gps, compass, etc.
	src/thread/*.c:		lcd, compass, gps, xbee, (maybe UI) threads
	src/*.c:			configurations
	
# log:
	- 4/9/17 : Dat 	- created main folder & readme 
			- integrated lcd, lec, photocell, UI, compass, buttons, rotary encoder
	- 4/10/17 : Dat - merged with current master
