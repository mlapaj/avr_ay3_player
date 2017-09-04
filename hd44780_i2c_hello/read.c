#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#pragma pack(1)


struct ym_header{

   char file_id[4];
   char check_string[8];
   uint32_t n_frames;
   uint32_t n_attribs;
   uint16_t n_digidrum;
   uint32_t freq;
   uint16_t player_rate;
   uint32_t loop;
   uint16_t skip;

};

typedef struct ym_header ym_header_s;


char *get_string(FILE *plik){
	fpos_t pos_start;
	char *str = 0;
	int howmany = 0;
	fgetpos(plik,&pos_start);
		char c;
	do {
		fread(&c,sizeof(char),1,plik);
		howmany++;
	} while (c != 0);
	fsetpos(plik,&pos_start);
	str = malloc(howmany);
	fread(str,howmany,1,plik);
	return str;

}

int main(int argc,char **argv){
	printf("YM read\n");
	FILE *plik;
	plik = fopen("cybrnoid.bin","rb");

	ym_header_s hdr;

	fread((void *) &hdr, sizeof(ym_header_s),1,plik);

	if (strncmp(hdr.file_id,"YM5!",4)!=0){
		printf("File id error\n");
		goto error;
	}
	if (strncmp(hdr.check_string,"LeOnArD!",9)!=0){
		printf("Format error\n");
	}
	hdr.n_attribs = __builtin_bswap32(hdr.n_attribs);
	hdr.n_frames = __builtin_bswap32(hdr.n_frames);
	hdr.n_digidrum = __builtin_bswap16(hdr.n_digidrum);
	hdr.freq = __builtin_bswap32(hdr.freq);
	hdr.player_rate = __builtin_bswap16(hdr.player_rate);
	hdr.loop = __builtin_bswap32(hdr.loop);
	hdr.skip = __builtin_bswap16(hdr.skip);

	printf("n_frames 0x%ld\n", hdr.n_frames);
	printf("n_attribs 0x%ld\n", hdr.n_attribs);
	printf("n_digidrum 0x%ld\n", hdr.n_digidrum);
	printf("frequency 0x%ld\n", hdr.freq);
	printf("player_rate 0x%ld\n", hdr.player_rate);
	printf("loop 0x%ld\n", hdr.loop);
	printf("skip 0x%ld\n", hdr.skip);
	fseek(plik,hdr.skip,SEEK_CUR);
	if (hdr.n_digidrum > 0)
	{
		// TODO: add handling
	}
	printf("File name: %s\n",get_string(plik));
	printf("Author: %s\n",get_string(plik));
	printf("Comment: %s\n",get_string(plik));



	long howmany = 0;
	char c[16];
	while (fread(&c,16,1,plik) == 1){
		howmany++;
	}
	printf("howmany: %d\n", howmany);

	fclose(plik);
	return 0;
error:
	fclose(plik);


}

