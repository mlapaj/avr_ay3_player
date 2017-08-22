#define F_CPU 16000000L
#include "i2c.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>


// bit configuration
// 0st bit - RS
// 1st bit - RW
// 2nd bit - E
// 3rd bit - KATODA
// 4th bit - D4
// 5th bit - D5
// 6th bit - D6
// 7th bit - D7

#define E_PIN  2
#define RS_PIN 0
#define RW_PIN 1

uint8_t port = 0;

int main(){
	i2c_init();
	port = 0; // all pins are low
	i2c_write(0x3F, port);
	_delay_ms(500);

	port |= _BV(3); // enable backlight

	// init procedure
	port |= _BV(5);
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);


	// init procedure
	port &= 15; // port and 1111 to store control values
	port |= _BV(5);
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	port &= 15; // port and 1111 to store control values
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);


	// CURSOR
	port &= 15; // port and 1111 to store control values
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	_delay_ms(10);

	port &= 15; // port and 1111 to store control values
	port |= (_BV(0) | _BV(1) | _BV(2) | _BV(3)) << 4;
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);




	// increment
	port &= 15; // port and 1111 to store control values
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	_delay_ms(10);

	port &= 15; // port and 1111 to store control values
	port |=  (_BV(1) | _BV(2))  << 4;
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);


	// clr scr
	port &= 15; // port and 1111 to store control values
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	_delay_ms(10);

	port &= 15; // port and 1111 to store control values
	port |=  (_BV(0))  << 4;
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);






	_delay_ms(10);
	char text[] = "hello";
	int i = 0;


	for (i = 0; i < strlen(text); i++){
		// write text
	    port &= 15; // port and 1111 to store control values
		port |= _BV(RS_PIN);
		port |= (((text[i] >> 4) & 0xF) << 4);
		i2c_write(0x3F, port);
		port |= _BV(E_PIN);
		i2c_write(0x3F, port);
		_delay_us(100);
		port &= ~_BV(E_PIN);
		i2c_write(0x3F, port);
		_delay_us(100);

	    port &= 15; // port and 1111 to store control values
		port |= (((text[i]) & 0xF) << 4);
		i2c_write(0x3F, port);
		port |= _BV(E_PIN);
		i2c_write(0x3F, port);
		_delay_us(100);

		port &= ~_BV(E_PIN);
		i2c_write(0x3F, port);
		_delay_us(100);

		port &= ~_BV(RS_PIN);
		i2c_write(0x3F, port);
	}



	return 0;
#if 0

	_delay_ms(1000);

	// LCD initialisaion
	DATA_PORT = _BV(5);
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);


	// LCD initialisaion
	DATA_PORT = _BV(5); // once again 4 bit
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);
	_delay_us(100);

	DATA_PORT = 0;
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);


	// CURSOR
	_delay_ms(10);

	DATA_PORT = 0;
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);

	DATA_PORT = (_BV(0) | _BV(1) | _BV(2) | _BV(3)) << 4;
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);

	_delay_ms(10);

	// increment mode
	DATA_PORT = 0;
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);

	DATA_PORT = (_BV(1) | _BV(2)) << 4;
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);


	_delay_ms(10);
	char text[] = "hello";
	int i = 0;

	for (i = 0; i < strlen(text); i++){
		// write text
		CONTROL_PORT |= _BV(RS_PIN);
		DATA_PORT = (((text[i] >> 4) & 0xF) << 4);
		CONTROL_PORT |= _BV(E_PIN);
		_delay_us(100);
		CONTROL_PORT &= ~_BV(E_PIN);
		_delay_us(100);
		CONTROL_PORT &= ~_BV(RS_PIN);

		_delay_us(500);
		CONTROL_PORT |= _BV(RS_PIN);
		DATA_PORT = (((text[i]) & 0xF) << 4);
		CONTROL_PORT |= _BV(E_PIN);
		_delay_us(100);
		CONTROL_PORT &= ~_BV(E_PIN);
		_delay_us(100);
		CONTROL_PORT &= ~_BV(RS_PIN);
	}
#endif
}

