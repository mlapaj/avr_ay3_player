/* "led.c" - programik do testowania œrodowiska WinAVR */
/* uk³ad ATmega 1MHz */
/* PB0,PB1 - diody LED; PD0 - przycisk */
#define F_CPU 16000000L

#include <stdio.h>
#include <util/delay.h>
#include "avr/pgmspace.h"
#include "avr/sfr_defs.h"
#include "i2c.h"
#include "hd44780.h"




int main(){
	_delay_ms(100);
	i2c_init();
	lcd_init();
	return 0;
	lcd_puts("Hello World...");
	lcd_goto(0x0);		
	return 0;
	lcd_clrscr();
/*
	uint8_t data_read;
	while(1){
	   if (i2c_write(0x3F,1 << 3) != 0) goto error;
	   _delay_ms(1000);
	   if (i2c_write(0x3F,0) != 0) goto error;
	   _delay_ms(1000);
	}
*/
	return 0;
error:
	PORTB = 0x20;
	return 0;
}

