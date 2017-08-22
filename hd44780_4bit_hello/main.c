#define F_CPU 1000000L
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define DATA_PORT PORTD

#define CONTROL_PORT PORTC
#define RS_PIN 5
#define RW_PIN 4
#define E_PIN  3

// Constants/Macros
#define PIN(x) (*(&x - 2))           // Address of Data Direction Register of Port X
#define DDR(x) (*(&x - 1))           // Address of Input Register of Port X

int main(){
	DDR(DATA_PORT) = 0xFF;
	DDR(CONTROL_PORT) |= _BV(E_PIN) | _BV(RS_PIN) | _BV(RW_PIN);
	CONTROL_PORT = 0x0;
	DATA_PORT = 0x0;

	_delay_ms(1000);

	// LCD initialisaion
	DATA_PORT = _BV(5);
	CONTROL_PORT |= _BV(E_PIN);
	_delay_us(100);
	CONTROL_PORT &= ~_BV(E_PIN);
	_delay_us(100);


	// LCD initialisaion
	DATA_PORT = _BV(5); // once again 4 bit and 2 lines
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
}

