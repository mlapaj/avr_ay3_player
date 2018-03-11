#include "common_settings.h"
#include "ym.h"
#include "debug.h"

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "pff.h"
//#define DEBUG 0

ym_header_s hdr;
int data_pos = 0;
int processed = 0;
char ym_tmp_str[255];
DWORD reg_pos[16];

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
	processed = 0;
	data_pos = 0;
	FRESULT retVal;
	UINT bytes_read;
	retVal = pf_open(filename);
	if (retVal != FR_OK){
		lcd_error("f err:%s",filename);
	}
	retVal = pf_read(&hdr,sizeof(ym_header_s),&bytes_read);
	if (retVal != FR_OK || bytes_read != sizeof(ym_header_s)){
		lcd_error("f rd err");
	}


	if (strncmp(hdr.file_id,"YM5!",4)!=0){
		lcd_error("YM5 err");
	}
	if (strncmp(hdr.check_string,"LeOnArD!",9)!=0){
		lcd_error("Format error");
	}
#ifdef DEBUG
	uart_debug("file_id and check_string verified successfully.");
#endif	
	hdr.attribs = __builtin_bswap32(hdr.attribs);
	hdr.n_frames = __builtin_bswap32(hdr.n_frames);
	hdr.n_digidrum = __builtin_bswap16(hdr.n_digidrum);
	hdr.freq = __builtin_bswap32(hdr.freq);
	hdr.player_rate = __builtin_bswap16(hdr.player_rate);
	hdr.loop = __builtin_bswap32(hdr.loop);
	hdr.skip = __builtin_bswap16(hdr.skip);
	int i = 0;
#ifdef DEBUG
	uart_debug("attribs: %d",hdr.attribs);
	uart_debug("n_frames: %d",hdr.n_frames);
	uart_debug("n_digidrum: %d",hdr.n_digidrum);
	uart_debug("frequency %ld",hdr.freq);
	uart_debug("loop %ld",hdr.loop);
	uart_debug("player rate %d",hdr.player_rate);
	uart_debug("skip: %d",hdr.skip);
#endif
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

	data_pos = sizeof(ym_header_s) + hdr.skip;
	char *s = get_string();
	data_pos += strlen(s) +1;
	uart_debug("File name: %s\n",s);
	s = get_string();
	data_pos += strlen(s) +1;
    uart_debug("Author: %s\n",s);
	s = get_string();
	data_pos += strlen(s) +1;
	uart_debug("Comment: %s\n",s);
//	hdr.attribs = 0;
	pf_lseek(data_pos);


	if (hdr.attribs & A_STREAMINTERLEAVED){
		uart_debug("Interleaved mode");
		for (int i = 0;i<16;i++){
			reg_pos[i] = data_pos + (i * hdr.n_frames);
		}
	}
#ifdef DEBUG
	uart_debug("File opened successfully.\n",s);
#endif
	return;
error:
	return;
}



uint8_t ym_get_registers(uint8_t *data,uint8_t howmany){
	UINT bytes_read;
	uint8_t buffer[64];
	if (hdr.attribs & A_STREAMINTERLEAVED){
		uint8_t cur_reg = 0;
		uint8_t cur = 0;
		DWORD pos;
		for (cur_reg = 0; cur_reg < 16; cur_reg++){
			pf_lseek(reg_pos[cur_reg] + processed);
		    pf_read(&buffer,howmany,&bytes_read);
			for (cur = 0; cur < howmany; cur++){
				data[16*cur + cur_reg] = buffer[cur];
			}
			if (bytes_read != howmany)
			{
				goto error;
			}
		}
		processed+=howmany;
	}
	else{
		pf_read(data,howmany*16,&bytes_read);
		if (bytes_read != howmany*16){
			goto error;
		}
	}
	return 0;
error:
	pf_lseek(data_pos);
	processed = 0;
	memset(data,0,16*howmany);
	uart_debug("get reg err");
	return -1;
	}


