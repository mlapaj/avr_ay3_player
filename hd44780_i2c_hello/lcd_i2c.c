#include "lcd_i2c.h"

uint8_t port = 0;


void lcd_command(uint8_t command){
	// clr scr
	port &= 15; // port and 1111 to store control values
	port |= (((command >> 4) & 0b1111) << 4);
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	_delay_ms(1);

	port &= 15; // port and 1111 to store control values
	port |= ((command & 0b1111) << 4);
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	_delay_ms(1);
}

void lcd_puts(char *text){
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
		_delay_us(100);
	}


	_delay_ms(10);
}

void lcd_clr(){
	lcd_command(0b1);
}

void lcd_home(){
	lcd_command(0b10);
}

void lcd_gotoxy(uint8_t x,uint8_t y){
	lcd_home();
	// go to new line
	if (y == 1){
		x = x + 40;
	}
	for (int i=0;i<x;i++){
		lcd_command(0b10100);
	}
}

void lcd_init(){
    // set 4 bit mode
	port |= _BV(3); // enable backlight
	// init procedure, set 4 bit mode
	port |= _BV(5);
	i2c_write(0x3F, port);
	port |= _BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);
	port &= ~_BV(E_PIN);
	i2c_write(0x3F, port);
	_delay_us(100);

	// set 4 bit mode once again with 4 bit mode
	lcd_command(0b101000); // 2 lines
	lcd_clr();
	lcd_home();
	lcd_command(0b1111); // set cursor
	lcd_command(0b110);  // increment mode
}

