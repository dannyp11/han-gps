/*
 * LCD.c
 *
 *  Created on: Feb 27, 2017
 *      Author: danny
 */

#include <avr/io.h>
#include <compat/twi.h>

#include <stdio.h>
#include <string.h>

#ifndef NOCHIBI
#include <hal.h>
#include "i2cconf.h"
#else
#include <util/delay.h>
#endif

#include "LCD.h"

// Find divisors for the UART0 and I2C baud rates
#ifndef FOSC
#define FOSC 7372800            // Clock frequency = Oscillator freq.
#endif
#define BDIV (FOSC / 100000 - 16) / 2 + 1    // Puts I2C rate just below 100kHz

/*
 * Low level codes here
 */
void i2c_init(uint8_t);
uint8_t i2c_io(uint8_t, uint8_t *, uint16_t, uint8_t *, uint16_t, uint8_t *,
		uint16_t);

/*
 * Portability
 */
#ifdef NOCHIBI
#define mSleepms(ms) _delay_ms(ms);
#define mSleepus(us) _delay_us(us);
#define LCD_ADDR  0x50
#else
#define LCD_ADDR  0x28
#define mSleepms(ms) chThdSleepMilliseconds(ms);
#define mSleepus(us) chThdSleepMicroseconds(us);
#endif

static uint8_t mI2Cio(uint8_t device_addr, uint8_t *wp, uint16_t wn,
		uint8_t *rp, uint16_t rn)
{
#ifdef NOCHIBI
	return i2c_io(device_addr, wp, wn, NULL, 0, rp, rn);
#else
	uint8_t result;
	i2cAcquireBus(&I2CD1);
	result = i2cMasterTransmitTimeout(&I2CD1, device_addr, wp, wn, rp, rn, 2);
	i2cReleaseBus(&I2CD1);
	return result;
#endif
}

/*
 * LCD APIs here
 */
// Start LCD APIs -------------------------------------------------------------
uint8_t LCDSendCommand(LCDCommand command)
{
	uint8_t status;
	uint8_t cmd[2];
	cmd[0] = 0xfe;

	switch (command)
	{
	case LCDON:
	{
		cmd[1] = 0x41;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepus(150);
	}
		break;
	case LCDOFF:
	{
		cmd[1] = 0x42;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepus(150);
	}
		break;
	case CLEARSCREEN:
	{
		cmd[1] = 0x51;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepms(150);
	}
		break;
	case CURSORHOME:
	{
		cmd[1] = 0x46;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepms(2);
	}
		break;
	case CURSORON:
	{
		cmd[1] = 0x4b;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepus(150);
	}
		break;
	case CURSOROFF:
	{
		cmd[1] = 0x4c;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepus(150);
	}
		break;
	case SHOWFIRMWARE:
	{
		cmd[1] = 0x70;
		status = mI2Cio(LCD_ADDR, cmd, 2, NULL, 0);
		mSleepms(5);
	}
		break;
	default:
		return 1;
		break;
	}

	return status;
}

uint8_t LCDSetBrightness(char level)
{
	uint8_t status;

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x53;
	cmd[2] = level;
	status = mI2Cio(LCD_ADDR, cmd, 3, NULL, 0);
	mSleepus(100);

	return status;
}

uint8_t LCDSetContrast(char level)
{
	uint8_t status;

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x52;
	cmd[2] = level;
	status = mI2Cio(LCD_ADDR, cmd, 3, NULL, 0);
	mSleepms(50);

	return status;
}

uint8_t LCDSetCursor(char line, char position)
{
	uint8_t status;
	char col = 0x00;
	if (line == 2)
	{
		col = 0x40;
	}
	else if (line == 3)
	{
		col = 0x14;
	}
	else if (line == 4)
	{
		col = 0x54;
	}

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x45;
	cmd[2] = col + position;
	status = mI2Cio(LCD_ADDR, cmd, 3, NULL, 0);
	mSleepus(150);

	return status;
}

void LCDInit()
{
#ifdef NOCHIBI
	i2c_init(BDIV);             // Initialize the I2C port
	mSleepms(200);
#else
	i2cStart(&I2CD1, &i2c_config);
	chThdSleepMilliseconds(200);
#endif

	LCDReset();
}

uint8_t LCDPrint(const char *msg)
{
	uint8_t status;

	uint8_t i;
	char len = strlen(msg);
	for (i = 0; i < (uint8_t) len; ++i)
	{
		status = mI2Cio(LCD_ADDR, (uint8_t *) &msg[i], 1, NULL, 0);
		mSleepus(150);
		if (status)
		{
			return status;
		}
	}

	// for filling the blank
	if (len < LCD_LINE_LEN)
	{
		char blank = ' ';
		for (i = 0; i < (uint8_t) (LCD_LINE_LEN -len); ++i)
		{
			status = mI2Cio(LCD_ADDR, (uint8_t *) &blank, 1, NULL, 0);
			mSleepus(150);
			if (status)
			{
				return status;
			}
		}
	}

	return status;
}

void LCDReset()
{
	LCDSendCommand(CLEARSCREEN);
	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CURSOROFF);
	LCDSetContrast(40);
	LCDSetBrightness(8);
}
// End LCD APIs -------------------------------------------------------------

/* ----------------------------------------------------------------------- */

/*
 i2c_io - write and read bytes to a slave I2C device

 This funtions write "an" bytes from array "ap" and then "wn" bytes from array
 "wp" to I2C device at address "device_addr".  It then reads "rn" bytes from
 the same device to array "rp".

 Return values (might not be a complete list):
 0    - Success
 0x20 - NAK received after sending device address for writing
 0x30 - NAK received after sending data
 0x38 - Arbitration lost with address or data
 0x48 - NAK received after sending device address for reading

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 This "i2c_io" I2C routine is an attempt to provide an I/O function for both
 reading and writing, rather than have separate functions.

 I2C writes consist of sending a stream of bytes to the slave device.  In some
 cases the first few bytes may be the internal address in the device, and then
 the data to be stored follows.  For example, EEPROMs like the 24LC256 require a
 two-byte address to precede the data.  The DS1307 RTC requires a one-byte
 address.

 I2C reads often consist of first writing one or two bytes of internal address
 data to the device and then reading back a stream of bytes starting from that
 address.  Some devices appear to claim that that reads can be done without
 first doing the address writes, but so far I haven't been able to get any to
 work that way.

 This function does writing and reading by using pointers to three arrays "ap",
 "wp", and "rp".  The function performs the following actions in this order:
 If "an" is greater than zero, then "an" bytes are written from array "ap"
 If "wn" is greater then zero, then "wn" bytes are written from array "wp"
 If "rn" is greater then zero, then "rn" byte are read into array "rp"
 Any of the "an", "wn", or "rn" can be zero.

 The reason for separate "ap" and "wp" arrays is that the address data can be
 taken from one array (ap), and then the write data from another (wp) without
 requiring that the contents be merged into one array before calling the
 function.  This means the following three calls all do exactly the same thing.

 i2c_io(0xA0, buf, 100, NULL, 0, NULL, 0);
 i2c_io(0xA0, NULL, 0, buf, 100, NULL, 0);
 12c_io(0xA0, buf, 2, buf+2, 98, NULL, 0);

 In all cases 100 bytes from array "buf" will be written to the I2C device at
 bus address 0xA0.

 A typical write with a 2-byte address is done with

 i2c_io(0xA0, abuf, 2, wbuf, 50, NULL, 0);

 A typical read with a 1-byte address is done with

 i2c_io(0xD0, abuf, 1, NULL, 0, rbuf, 20);
 */

uint8_t i2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an, uint8_t *wp,
		uint16_t wn, uint8_t *rp, uint16_t rn)
{
	uint8_t status, send_stop;

	status = 0;
	send_stop = 0;

	if (an > 0 || wn > 0)
	{
		send_stop = 1;

		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); // Send start condition
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & TW_NO_INFO;
		if (status != TW_START)                 // Check that START was sent OK
			return (status);

		TWDR = device_addr & 0xfe;          // Load device address and R/W = 0;
		TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & TW_NO_INFO;
		if (status != TW_MT_SLA_ACK)
		{               // Check that SLA+W was sent OK
			if (status == TW_MT_SLA_NACK)             // Check for NAK
				goto nakstop;
			// Send STOP condition
			return (status);                 // Otherwise just return the status
		}

		// Write "an" data bytes to the slave device
		while (an-- > 0)
		{
			TWDR = *ap++;                   // Put next data byte in TWDR
			TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
			while (!(TWCR & (1 << TWINT)))
				; // Wait for TWINT to be set
			status = TWSR & 0xf8;
			if (status != TW_MT_DATA_ACK)
			{           // Check that data was sent OK
				if (status == TW_MT_DATA_NACK)         // Check for NAK
					goto nakstop;
				// Send STOP condition
				return (status);             // Otherwise just return the status
			}
			mSleepus(50);
		}

		status = 0;                         // Set status value to successful
	}

	nakstop:                              // Come here to send STOP after a NAK
	if (send_stop)
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Send STOP condition

	return (status);
}

/*
 i2c_init - Initialize the I2C port
 */
void i2c_init(uint8_t bdiv)
{
	TWSR = 0;                           // Set prescalar for 1
	TWBR = bdiv;                        // Set bit rate register
}

/* ----------------------------------------------------------------------- */
