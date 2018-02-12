#include "uart.h"
extern char debug_text[100];



#define debug(...) {sprintf(debug_text,__VA_ARGS__); uart_send(debug_text); uart_send("\r\n");}
#define debug_noline(...) {sprintf(debug_text,__VA_ARGS__); uart_send(debug_text);}
