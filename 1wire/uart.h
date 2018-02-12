#ifndef uart_h
#define uart_h
#include "common_settings.h"
#include <avr/io.h>
#include <util/delay.h>

// define some macros
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR


void uart_init (void);
void uart_transmit (unsigned char data);
void uart_send(char *data);


#endif
