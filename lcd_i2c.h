#ifndef _LCD_I2C_FILE
#define _LCD_I2C_FILE

#include "common_settings.h"
#include "i2c.h"
#include <util/delay.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

// bit configuration
// 0st bit - RS
// 1st bit - RW
// 2nd bit - E
// 3rd bit - KATODA
// 4th bit - D4
// 5th bit - D5
// 6th bit - D6
// 7th bit - D7

#define E_PIN  2
#define RS_PIN 0
#define RW_PIN 1


void lcd_command(uint8_t command);
void lcd_puts(char *text);
void lcd_clr();
void lcd_home();
void lcd_gotoxy(uint8_t x,uint8_t y);
void lcd_init(int type);


#endif
