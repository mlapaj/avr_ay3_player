#ifndef _I2C_FILE
#define _I2C_FILE


#define LCD_MUX_ADDR 0x3F

#include <stdio.h>
int i2c_read(uint8_t addr,uint8_t *data);
int i2c_write(uint8_t addr,uint8_t data);
void i2c_init();




#endif
