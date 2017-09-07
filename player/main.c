#include "common_settings.h"
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "pff.h"
#include "ym.h"
#include "debug.h"
#include "ay3_89xx.h"

#define REGS_IN_BUF 8
uint8_t ay3_data0[REGS_IN_BUF*16];
int data0_processed = 0;
uint8_t ay3_data1[REGS_IN_BUF*16];
int data1_processed = 0;

extern ym_header_s hdr;

void my_delay_ms(int n) {
 while(n--) {
  _delay_ms(1);
 }
}
static int processed_frame = 0;

ISR(TIMER0_COMPA_vect){
	static int c = 0;
	if (c == 2){
		static int processed = 0;
		static int current_buffer = 0;
		if (processed >= REGS_IN_BUF){
			if (current_buffer == 0){
				if (data1_processed == 1){
					uart_debug("tm err 0");
					return;
				}
				data0_processed = 1;
				current_buffer = 1;
			}
			else
			{
				if (data0_processed == 1){
					uart_debug("tm err 1");
					return;
				}
				data1_processed = 1;
				current_buffer = 0;
			}
			processed = 0;
		}
		int reg = 0;
		for (reg=0;reg<14;reg++){ // we use 14 first registers
			if (current_buffer == 0){
				write_psg_register(reg,ay3_data0[(processed * 16) + reg]);
			} else {
				write_psg_register(reg,ay3_data1[(processed * 16) + reg]);
			}
		}
		processed++;
		processed_frame++;
		// XOR PORTA with 0x01 to toggle the second bit up
		c = 0;
	}

	c++;
}

/*
ISR(TIMER0_OVF_vect) {
}
*/

int main(){
	int i;
	int frame,reg;
	ay3_init();
	uart_init();
	i2c_init();
	lcd_init();
	uart_debug("YM player (C) 2017 Marcin Lapaj");

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


    OCR0A = 157;
    TCCR0A |= (1 << WGM01);
	TIMSK0 |=(1<<OCIE0A) ;
	TCCR0B |= _BV(CS02) | _BV(CS00);
	ym_get_registers(ay3_data0,REGS_IN_BUF);
	ym_get_registers(ay3_data1,REGS_IN_BUF);
	sei();
	int total_frames = 0;
	int update = 0;
	while(1){
		cli();
		if (data0_processed == 1){
		sei();
			ym_get_registers(ay3_data0,REGS_IN_BUF);
			data0_processed = 0;
		}
		else if (data1_processed == 1){
		sei();
			ym_get_registers(ay3_data1,REGS_IN_BUF);
			data1_processed = 0;
		}
		else{
		sei();
		}
		if (processed_frame - update > 20){
			update = processed_frame;
			lcd_gotoxy(0,1);
			lcd_debug("f %d / %d", processed_frame,hdr.n_frames);
			uart_debug("f %d / %d", processed_frame,hdr.n_frames);
		}

	}
	uart_debug("processed total frames: %d",total_frames);
	return 0;

}


