#include "common_settings.h"
#include "ym.h"
#include "debug.h"

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "pff.h"

ym_header_s hdr;

char ym_tmp_str[255];

char *get_string(){
	char c;
	UINT bytes_read;
	int i = 0;
	do{
		pf_read(&c,1,&bytes_read);
		ym_tmp_str[i] = c; 
		i++;
	}while (c != 0);

	return ym_tmp_str;

}

void ym_load(char *filename){
	FRESULT retVal;
	UINT bytes_read;
	lcd_debug("BGN");
	retVal = pf_open(filename);
	if (retVal != FR_OK){
		lcd_error("f err:%s",filename);
	}
	retVal = pf_read(&hdr,sizeof(ym_header_s),&bytes_read);
	if (retVal != FR_OK || bytes_read != sizeof(ym_header_s)){
		lcd_error("f rd err");
	}

	lcd_debug("READ");

	if (strncmp(hdr.file_id,"YM5!",4)!=0){
		lcd_error("YM5 err");
	}
	if (strncmp(hdr.check_string,"LeOnArD!",9)!=0){
		lcd_error("Format error");
	}

	uart_debug("file_id and check_string verified successfully.");
	lcd_debug("VFY");
	hdr.n_attribs = __builtin_bswap32(hdr.n_attribs);
	hdr.n_frames = __builtin_bswap32(hdr.n_frames);
	hdr.n_digidrum = __builtin_bswap16(hdr.n_digidrum);
	hdr.freq = __builtin_bswap32(hdr.freq);
	hdr.player_rate = __builtin_bswap16(hdr.player_rate);
	hdr.loop = __builtin_bswap32(hdr.loop);
	hdr.skip = __builtin_bswap16(hdr.skip);
	int i = 0;
	uart_debug("n_attribs: %d",hdr.n_attribs);
	uart_debug("n_frames: %d",hdr.n_frames);
	uart_debug("n_digidrum: %d",hdr.n_digidrum);
	uart_debug("frequency %ld",hdr.freq);
	uart_debug("loop %ld",hdr.loop);
	uart_debug("player rate %d",hdr.player_rate);
	uart_debug("skip: %d",hdr.skip);
	for (i=0;i<hdr.skip;i++)
	{
		char c = 0;
		retVal = pf_read(&c,1,&bytes_read);
		// add error handler
	}
	if (hdr.n_digidrum > 0)
	{
		// TODO: add handling
	}
	uart_debug("File name: %s\n",get_string());
    uart_debug("Author: %s\n",get_string());
	uart_debug("Comment: %s\n",get_string());

	if (hdr.n_attribs & A_STREAMINTERLEAVED){
		uart_debug("Interleaved mode");
	}
	lcd_debug("OK");
	return;
error:
	return;
}


uint8_t ym_get_registers(uint8_t *data){
	UINT bytes_read;
	pf_read(data,16,&bytes_read);
	if (bytes_read == 16){
		return 0;
	}
	else
	{
		uart_debug("get reg err");
		return -1;
	}
}
