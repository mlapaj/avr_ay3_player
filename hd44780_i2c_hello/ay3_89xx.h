#ifndef AY3_89XX_H
#define AY3_89XX_H

#include "common_settings.h"
#include "ay3_89xx_settings.h"
#include <stdint.h>
#include <inttypes.h>


void ay3_init();
uint8_t read_psg_register(uint8_t psg_reg);
void write_psg_register(uint8_t psg_reg,uint8_t val);
void psg_reset();



#endif
