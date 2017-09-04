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



int  deInterleave(ym_header_s hdr, uint8_t *buf, uint8_t *buf_new)
{
	uint32_t	nextPlane[32];
	uint8_t	*pW;
	uint32_t	j,k;
	int streamInc = 16;
	pW = buf_new;

	for (j=0;j<streamInc;j++){ nextPlane[j] = hdr.n_frames*j;
		printf("nextPlane[%d] %d\n",j,nextPlane[j]);
	}

	pW = buf_new;
	for (j=0;j<nextPlane[1];j++)
	{
		for (k=0;k<streamInc;k++)
		{
//			printf("pw[%d] = pDataStream [j %d + nextPlane[%d] %d]\n",k,j,k,nextPlane[k]);
			pW[k] = buf[j + nextPlane[k]];
		}
		pW += streamInc;
	}
	return 1;
}



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
	uint8_t *buf;
	uint8_t *buf_new;
	printf("YM read\n");
	FILE *plik;
	FILE *out_plik;
	plik = fopen("cybnoid2.bin","rb");
	out_plik = fopen("out2.bin","wb");

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

	printf("n_frames %ld\n", hdr.n_frames);
	printf("n_attribs %ld\n", hdr.n_attribs);
	printf("n_digidrum %ld\n", hdr.n_digidrum);
	printf("frequency %ld\n", hdr.freq);
	printf("player_rate %ld\n", hdr.player_rate);
	printf("loop %ld\n", hdr.loop);
	printf("skip %ld\n", hdr.skip);
	fseek(plik,hdr.skip,SEEK_CUR);
	if (hdr.n_digidrum > 0)
	{
		// TODO: add handling
	}
	printf("File name: %s\n",get_string(plik));
	printf("Author: %s\n",get_string(plik));
	printf("Comment: %s\n",get_string(plik));

	long pos;
	pos = ftell(plik);
	printf("pos %lx\n",pos);
	fseek (plik, 0, SEEK_SET);
	
	char *buf_hdr = malloc(pos);
	fread(buf_hdr,pos,1,plik);
	fwrite(buf_hdr,pos,1,out_plik);
	free(buf_hdr);

	buf = malloc((hdr.n_frames * 16) + 4);
	buf_new = malloc((hdr.n_frames * 16) + 4);
	
	fread(buf,(hdr.n_frames*16)+4,1,plik);
	deInterleave(hdr,buf,buf_new);
	fwrite(buf_new,(hdr.n_frames*16)+4,1,out_plik);


	free(buf);
	free(buf_new);
	fclose(plik);
	fclose(out_plik);
	return 0;
error:
	fclose(plik);


}

