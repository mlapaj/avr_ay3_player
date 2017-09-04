#ifndef ym_h
#define ym_h

#include <stdint.h>



enum
{
        A_STREAMINTERLEAVED = 1,
        A_DRUMSIGNED = 2,
        A_DRUM4BITS = 4,
        A_TIMECONTROL = 8,
        A_LOOPMODE = 16,
};




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


void ym_load(char *filenname);

uint8_t ym_get_registers(uint8_t *data);

#endif
