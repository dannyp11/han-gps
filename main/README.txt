# run: 
	make ; make program
	
# code structure:
	include/*.h : 		all project include files, doesn't separate sw, hw, etc.
	src/hardware/*.c:	driver for components like lcd, led, gps, compass, etc.
	src/thread/*.c:		UI, parser, computing threads
	src/*.c:			configurations
	
# log:
	- 4/9/17 : Dat 	- created main folder & readme 
			- integrated lcd, lec, photocell, UI, compass, buttons, rotary encoder
	- 4/10/17 : Dat - merged with current master
	- 4/11/17 : Rongcui 
		- Implemented, but did not test core alorithm, naive version
		- Merged XBee and GPS thread into Parser thread
		- Reduced memory usage to ~1.5kB
		- Interfaced with UI thread
		- Changed Soft Serial to use TIM1. Not tested.
	- 4/18/17 : Rongcui
		- Fixed UI/Parser conflict
		- Changed some snprintf() to chsnprintf()
		- Added I2C AcquireBus/ReleaseBus
		
	- 4/24/17: All
		- Completed first release version
		- Basic functionality works flawlessly
		- Optimal tx speed for xbees is configured
