#include "uart.h"
#include "lcd_i2c.h"
extern char debug_text[100];

#define lcd_print(...) {sprintf(debug_text,__VA_ARGS__); lcd_puts(debug_text);}
#define lcd_debug(...) {lcd_gotoxy(0,0); sprintf(debug_text,__VA_ARGS__); lcd_puts(debug_text);}
#define lcd_error(...) {lcd_gotoxy(0,1); sprintf(debug_text,__VA_ARGS__); lcd_puts(debug_text); while(1);}



#define uart_debug(...) {sprintf(debug_text,__VA_ARGS__); uart_send(debug_text); uart_send("\r\n");}
#define uart_debug_noline(...) {sprintf(debug_text,__VA_ARGS__); uart_send(debug_text);}
