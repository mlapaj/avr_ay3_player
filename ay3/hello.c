/* "led.c" - programik do testowania œrodowiska WinAVR */
/* uk³ad ATmega 1MHz */
/* PB0,PB1 - diody LED; PD0 - przycisk */

#define F_CPU 1000000L
#include <stdio.h>
#include <util/delay.h>
#include "hd44780.h"
#include "ay3_89xx.h"
#if 0

#endif


int main(void)
{
	_delay_ms(1);
	ay3_init();
	lcd_init();
	lcd_clrscr();
	lcd_puts("Hello World...");
	lcd_goto(0x40);
	lcd_puts("Bye bye...");
	uint8_t val = 0;
	int i = 0;

	write_psg_register(0,8);
	write_psg_register(1,8);
	write_psg_register(2,8);
	write_psg_register(3,8);
	write_psg_register(4,8);
	write_psg_register(5,8);
	write_psg_register(6,8);
	write_psg_register(7,0x38);
	for (i = 10;i < 16; i++){
		write_psg_register(i,0);
	}


	int k = 0;
	int out_k = 0;
	char tmp[20];
	while (1){
		if (k>0xF){ 
			k = 0;
		}
		write_psg_register(8,k);
		lcd_clrscr();
	    lcd_goto(0x0);
		sprintf(tmp,"k: %d",k);
   	    lcd_puts(tmp);
		out_k = read_psg_register(8);
	    lcd_goto(0x40);
		sprintf(tmp,"read_k: %d",out_k);
   	    lcd_puts(tmp);
		_delay_ms(500);
		k+=1;
	}

}
