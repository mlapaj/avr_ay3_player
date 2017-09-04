#include "common_settings.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "pff.h"
#include "ym.h"
#include "debug.h"
#include "ay3_89xx.h"


uint8_t ay3_data[256];

void my_delay_ms(int n) {
 while(n--) {
  _delay_ms(1);
 }
}

int main(){
	int i;
	int frame,reg;
	ay3_init();
	uart_init();
	i2c_init();
	lcd_init();
	uart_debug("YM player (C) 2017 Marcin Lapaj");
	TCCR0A |= (1 << CS00);
	TCCR0B |= _BV(CS02) | _BV(CS00);;
	DIR dj;
	FRESULT retVal;
	FATFS fs;
	retVal = pf_mount (&fs);
	if (retVal != FR_OK){
		lcd_error("mt mnt nok: %d",retVal);
	}
	uart_debug("SD Card mounted");
	retVal = pf_opendir (&dj,"");
	if (retVal != FR_OK){
		lcd_error("od nok code: %d",retVal);
	}
	uart_debug("Main directory opened");
	uart_debug("File list");
	FILINFO fno;
	//while (1)
	{
		retVal = pf_readdir (&dj, &fno); /* Read a directory item from the open directory */
		if (retVal != FR_OK){
			lcd_error("rd nok code: %d",retVal);
		}
		if (fno.fname[0] == 0){
			//break;
		}
		lcd_debug("fn:%s",fno.fname);
		uart_debug(fno.fname);
		_delay_ms(500);
	}
	lcd_clr();
	uart_debug("opening file");

	ym_load("CYBRNOID.BIN");

	int total_frames = 0;
	
	while(1){
		TCNT0 = 0;
		if (ym_get_registers(ay3_data)!= 0)
			break;

		for (reg=0;reg<14;reg++){ // we use 14 first registers
			write_psg_register(reg,ay3_data[reg]);
		}
		// uart_debug("Timer val %d",TCNT0 / 16);
		// for 16 MHZ 16 ticks it is a 1ms
		// frame rate
		my_delay_ms(19 - TCNT0/16);
		total_frames += 16;
	}
	uart_debug("processed total frames: %d",total_frames);
	return 0;

}


