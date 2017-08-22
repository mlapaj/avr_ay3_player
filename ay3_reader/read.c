#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma pack(1)
struct ay_header{

   char FileID[4];			//'ZXAY'
   char TypeID[4];		//'EMUL'
   uint8_t FileVersion;			//Version of song file (release version)
   uint8_t PlayerVersion;		//Required player version. Micro Speccy
   uint16_t PSpecialPlayer;
   uint16_t PAuthor;
   uint16_t PMisc;
};

typedef struct ay_header ay_header_s;


int main(int argc,char **argv){
	printf("AY3 read\n");
	FILE *plik;
	plik = fopen("cybernoid.ay","r");
	ay_header_s hdr;
	fread((void *)&hdr, sizeof(ay_header_s), 1, plik);
	if (strncmp(hdr.FileID,"ZXAY",sizeof(hdr.FileID) != 0)){
			printf("Invalid header fid\n");
	}
	if (strncmp(hdr.TypeID,"EMUL",sizeof(hdr.TypeID) != 0)){
			printf("Invalid header lw\n");
	}
	printf("File Version %d\n",hdr.FileVersion);
	printf("Player Version %d\n",hdr.PlayerVersion);
	printf("Player Special Version %d\n",hdr.PSpecialPlayer);
	printf("Author offset %d\n",hdr.PAuthor);
	printf("Misc offset %d\n",hdr.PMisc);

	printf("Author:");
	// read author
	char c = 0;
	fseek(plik, hdr.PAuthor, 0); 
	do
	{
		fread(&c,sizeof(char),1,plik);
		printf(".%c",c);
		
	} while (c!=0);
	printf("\n");



	fclose(plik);


}

