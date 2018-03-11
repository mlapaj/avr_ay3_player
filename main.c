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

int int_processed = 0;
int int_current_buffer = 0;

void my_delay_ms(int n) {
 while(n--) {
  _delay_ms(1);
 }
}
int processed_frame = 0;

void reset_interrupt_handler(){
	int_processed = 0;
	int_current_buffer = 0;
	processed_frame = 0;
}


ISR(TIMER0_COMPA_vect){
	static int c = 0;
	if (c == 2){
		if (int_processed >= REGS_IN_BUF){
			if (int_current_buffer == 0){
				if (data1_processed == 1){
					uart_debug("tm err 0");
					return;
				}
				data0_processed = 1;
				int_current_buffer = 1;
			}
			else
			{
				if (data0_processed == 1){
					uart_debug("tm err 1");
					return;
				}
				data1_processed = 1;
				int_current_buffer = 0;
			}
			int_processed = 0;
		}
		int reg = 0;
		for (reg=0;reg<14;reg++){ // we use 14 first registers
			if (int_current_buffer == 0){
				write_psg_register(reg,ay3_data0[(int_processed * 16) + reg]);
			} else {
				write_psg_register(reg,ay3_data1[(int_processed * 16) + reg]);
			}
		}
		int_processed++;
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

int play_file(char *file){
	ym_load(file);
	OCR0A = 157;
	TCCR0A |= (1 << WGM01);
	TIMSK0 |=(1<<OCIE0A) ;
	TCCR0B |= _BV(CS02) | _BV(CS00);
	ym_get_registers(ay3_data0,REGS_IN_BUF);
	ym_get_registers(ay3_data1,REGS_IN_BUF);
	sei();
	int total_frames = 0;
	int update = 0;
	int update_button = 0;
	uint8_t val = 0;
	uint8_t prev_val = 0;
	i2c_write(0x3C, 0);
	uint8_t pause = 0;
	// get data from file to buffer
	while(processed_frame < hdr.n_frames){
		if (pause == 0){
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
		}
		// update button status and react on buttons
		if (processed_frame - update_button > 5 || pause){
			i2c_read(0x3C, &val);
			update_button = processed_frame;
			lcd_gotoxy(0,1);
			if (prev_val != val){
				if (val == 2){
					if (pause == 0){
						cli();
						pause = 1;
						psg_reset();
					}
					else{
						sei();
						pause = 0;
					}
				} else if (val == 4){
					// stop playing music
					cli();
					pause = 1;
					psg_reset();
					return 4;
				} else if (val == 1){
					// stop playing music
					cli();
					pause = 1;
					psg_reset();
					return 1;
				}
				prev_val = val;
			}
		}
		// update LCD status about song
		if (processed_frame - update > 40){
			update = processed_frame;
			lcd_gotoxy(0,1);
			lcd_print("f %d / %d", processed_frame,hdr.n_frames);
		}

	}
	// song has finished, exit function
	cli();
	uart_debug("processed total frames: %d",total_frames);
	return 0;


}



int find_song(int req,char *songName){
	int songNo = 0;
	DIR dj;
	FRESULT retVal;
	retVal = pf_opendir (&dj,"");
	if (retVal != FR_OK){
		lcd_error("od nok code: %d",retVal);
	}
	uart_debug("Main directory opened");
	uart_debug("File list");
	FILINFO fno;
	while (1)
	{
		retVal = pf_readdir (&dj, &fno); /* Read a directory item from the open directory */
		if (retVal != FR_OK){
			lcd_error("rd nok code: %d",retVal);
			uart_debug("rd nok code: %d",retVal);
		}
		if (fno.fname[0] == 0){
			break;
		}
		if (strlen(fno.fname) > 4){
			if ((strcmp(".bin",fno.fname + strlen(fno.fname)-4)==0) || 
					(strcmp(".BIN",fno.fname + strlen(fno.fname)-4)==0)) 
			{
				lcd_debug("fn:%s",fno.fname);
				if (songNo == req){
					strcpy(songName,fno.fname);
					return 0;
				}
				songNo++;
			}
		}
	}
	return -1;
}



int main(){
	int i;
	int frame,reg;
	char filename[255];
	int cur_song = 0;
	int ret_val = 0;
	ay3_init();
	uart_init();
	i2c_init();
	unsigned char ResetSrc = MCUSR;   // save reset source
	MCUSR = 0x00;  // cleared for next reset detection

	// check reset type
	if ((ResetSrc == 7) || (ResetSrc == 5)){
		// initialize LCD display 
		// and enable 4 bit mode
		lcd_init(0);
	}
	else
	{
		// 4 bit mode already enabled
		// re-initialize display
		lcd_init(1);
	}

	uart_debug("YM player (C) 2017 Marcin Lapaj");
	uart_debug("reset type %d",ResetSrc);


	FRESULT retVal;
	FATFS fs;
	retVal = pf_mount (&fs);
	if (retVal != FR_OK){
		lcd_error("mt mnt nok: %d",retVal);
	}
	uart_debug("SD Card mounted");
	lcd_clr();
	uart_debug("opening file");

	while (1){
		reset_interrupt_handler();
		psg_reset();
	    uart_debug("find %d song",cur_song);
		if (find_song(cur_song,filename) == 0){
			uart_debug("plaing %d song %s",cur_song,filename);
			lcd_clr();
			lcd_gotoxy(0,0);
			lcd_print(filename);
			ret_val = play_file(filename);
			memset(filename,0,sizeof(filename));
			if ((ret_val == 0) || (ret_val == 1)){
				uart_debug("playing next song...");
				cur_song++;
			}
			if (ret_val == 4){
				uart_debug("playing prev song...");
				cur_song --;
				if (cur_song < 0){
					cur_song = 0;
				}

			}
		} else {
			uart_debug("song not found, starting from zero...");
			cur_song = 0;
		}
	}

}


