#include "uart.h"

void uart_init (void)
{
    UBRR0H = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRR0L = BAUDRATE;                           // set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);                // enable receiver and transmitter
    UCSR0C|= /*(1<<URSEL)|*/(1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format
}

void uart_transmit (unsigned char data)
{
    while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
    UDR0 = data;                                   // load data in the register
}


void uart_send (char *data)
{
	uint8_t i = 0;
	while (data[i]){
		uart_transmit(data[i]);
		i++;
	}
}
