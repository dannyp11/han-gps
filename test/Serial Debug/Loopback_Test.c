#include <avr/io.h>
#include <stdint.h>

#define BAUD_RATE 9600

/*
	serial_init - Initialize the USART port
*/
void serial_init(unsigned short ubrr) {
	UBRR0 = ubrr;			// set baud rate
	UCSR0B |= (1 << TXEN0); // Turn on transmitter
	UCSR0B |= (1 << RXEN0); // Turn on receiver
	UCSR0C = (3 << UCSZ00); // Set for async operation, no parity, one stop bit, 8 data  bits
}

/*
	serial_out - Output a byte to the USART0 port
*/
void serial_out(char ch)
{
	while ((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = ch;
}

/*
	serial_in - Read a byte from the USART0 and return it
*/
char serial_in()
{
	while ( !(UCSR0A & (1 << RXC0)) );
	return UDR0;
}

int main(void) {
	serial_init(BAUD_RATE);

	while(1) {
		serial_in();
		serial_out();
	}
}
