#include "common_settings.h"
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "debug.h"
#define PIN(x) (*(&x - 2))
#define DDR(x) (*(&x - 1))


unsigned char crc88540_table[256] = {
    0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
  157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
   35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
  190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
   70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
  219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
  101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
  248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
  140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
   17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
  175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
   50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
  202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
   87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
  233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
  116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};

unsigned char one_wire_crc8(unsigned char *data, unsigned char count)
{
    unsigned char result=0; 

    while(count--) {
      result = crc88540_table[result ^ *data++];
    }
    
    return result;
}


void one_wire_init(){
		DDR(PORTB) |= _BV(1);
		PORTB |= _BV(1);
		_delay_ms(50);
}

uint8_t one_wire_reset(){
	uint8_t val;
	PORTB &= ~_BV(1);
	_delay_us(480);
	DDR(PORTB) &= ~_BV(1);
	_delay_us(70);
	val = PINB & _BV(1);
	DDR(PORTB) |= _BV(1);
	PORTB |= _BV(1);
	_delay_us(100);
	return (val == 0);
}

void one_wire_send(uint8_t byte){
	uint8_t i = 0;
	DDR(PORTB) |= _BV(1);
	for (i=0;i<8;i++){
		PORTB &= ~_BV(1);
		_delay_us(6);
		if (byte >> i & 1){
			PORTB |= _BV(1);
			_delay_us(64);
		}
		else{
			PORTB &= ~_BV(1);
			_delay_us(60);
			PORTB |= _BV(1);
			_delay_us(10);
		}
	}
	_delay_us(100);
}

uint8_t one_wire_receive(int bits){
		uint8_t result = 0;
		uint8_t val = 0;
		uint8_t i = 0;

		for (i=0;i<bits;i++){
			PORTB &= ~_BV(1);
			_delay_us(6);
			DDR(PORTB) &= ~_BV(1);
			_delay_us(9);
			val = PINB & _BV(1);
			if (val){
				result |= _BV(i);
			}
			_delay_us(55); // Complete the time slot and 10us recovery
			PORTB |= _BV(1);
			DDR(PORTB) |= _BV(1);
		}
		return result;
}


uint8_t one_wire_receive_byte(){
	return one_wire_receive(8);
}

uint8_t one_wire_read_rom(uint8_t *data){
	one_wire_send(0x33);
	for (int i=0;i<8;i++){
		data[i] = one_wire_receive_byte();
	}
	if (one_wire_crc8(data,7) == data[7]){
		return 1;
	}
	return 0;
}
void one_wire_search(){
	uint8_t data = 0;
	one_wire_send(0xF0);
	data = one_wire_receive(2);
	debug("ret: %d", data);

}

int main(void)
{
	uint8_t data[8];
	uart_init();
	one_wire_init();
	uint8_t val = 0;
	while(1){
		debug("reset %d",one_wire_reset());
		one_wire_search();
#if 0
		if (one_wire_read_rom(data)){
			for (int i=0;i<8;i++){
				debug_noline("0x%x ",data[i])
			}
			debug("");
		}
#endif
		_delay_ms(2000);
	}
}