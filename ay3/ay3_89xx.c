#include "avr/pgmspace.h"
#include "ay3_89xx.h"

#include <util/delay.h>
#include "avr/sfr_defs.h"

#define PIN(x) (*(&x - 2))
#define DDR(x) (*(&x - 1))

void set_da_output(){
#ifdef USE_ONE_DATA_PORT
		DDR(DA_PORT) = 0xFF;
#else
/*
		DDR(DA0_PORT)|=_BV(DA0_PIN);
		DDR(DA1_PORT)|=_BV(DA1_PIN);
		DDR(DA2_PORT)|=_BV(DA2_PIN);
		DDR(DA3_PORT)|=_BV(DA3_PIN);
		DDR(DA4_PORT)|=_BV(DA4_PIN);
		DDR(DA5_PORT)|=_BV(DA5_PIN);
		DDR(DA6_PORT)|=_BV(DA6_PIN);
		DDR(DA7_PORT)|=_BV(DA7_PIN);
*/
#endif
}


void set_da_input(){
#ifdef USE_ONE_DATA_PORT
		DA_PORT = 0x00;
		DDR(DA_PORT) = 0x00;
#else
/*
		DDR(DA0_PORT)&=~_BV(DA0_PIN);
		DDR(DA1_PORT)&=~_BV(DA1_PIN);
		DDR(DA2_PORT)&=~_BV(DA2_PIN);
		DDR(DA3_PORT)&=~_BV(DA3_PIN);
		DDR(DA4_PORT)&=~_BV(DA4_PIN);
		DDR(DA5_PORT)&=~_BV(DA5_PIN);
		DDR(DA6_PORT)&=~_BV(DA6_PIN);
		DDR(DA7_PORT)&=~_BV(DA7_PIN);
*/
#endif
}

void set_psg_inactive(){
	uint8_t val = PSG_B_PORT;
	val &= ~_BV(PSG_BDIR_PIN);
	val |= _BV(PSG_BC1_PIN);
	PSG_B_PORT = val;
	_delay_us(20);
}

void set_psg_read(){
	uint8_t val = PSG_B_PORT;
	val &= ~_BV(PSG_BDIR_PIN);
	val |= _BV(PSG_BC1_PIN);
	PSG_B_PORT = val;
	_delay_us(20);
}

void set_psg_write(){
	uint8_t val = PSG_B_PORT;
	val |= _BV(PSG_BDIR_PIN);
	val &= ~_BV(PSG_BC1_PIN);
	PSG_B_PORT = val;
	_delay_us(20);
}


void psg_latch(){
	uint8_t val = PSG_B_PORT;
	val |= _BV(PSG_BC1_PIN);
	val |= _BV(PSG_BDIR_PIN);
	PSG_B_PORT = val;
	_delay_us(20);
}


void write_data_port(uint8_t value){
#ifdef USE_ONE_DATA_PORT
	DA_PORT = value;
#else
/*
	(value & 1) ? (DA0_PORT|=_BV(DA0_PIN)) : (DA0_PORT&=~_BV(DA0_PIN));
	((value >> 1) & 1) ? (DA1_PORT|=_BV(DA1_PIN)) : (DA1_PORT&=~_BV(DA1_PIN));
	((value >> 2) & 1) ? (DA2_PORT|=_BV(DA2_PIN)) : (DA1_PORT&=~_BV(DA2_PIN));
	((value >> 3) & 1) ? (DA3_PORT|=_BV(DA3_PIN)) : (DA1_PORT&=~_BV(DA3_PIN));
	((value >> 4) & 1) ? (DA4_PORT|=_BV(DA4_PIN)) : (DA1_PORT&=~_BV(DA4_PIN));
	((value >> 5) & 1) ? (DA5_PORT|=_BV(DA5_PIN)) : (DA1_PORT&=~_BV(DA5_PIN));
	((value >> 6) & 1) ? (DA6_PORT|=_BV(DA6_PIN)) : (DA1_PORT&=~_BV(DA6_PIN));
	((value >> 7) & 1) ? (DA7_PORT|=_BV(DA7_PIN)) : (DA1_PORT&=~_BV(DA7_PIN));
*/
#endif
}


uint8_t read_data_port(){
	uint8_t retVal = 0;
#ifdef USE_ONE_DATA_PORT
	retVal = PIN(DA_PORT);
#else
/*
	((PIN(DA0_PORT) >> DA0_PIN) & 1) ? (retVal |= _BV(0)) : (retVal &= ~_BV(0));
	((PIN(DA1_PORT) >> DA1_PIN) & 1) ? (retVal |= _BV(1)) : (retVal &= ~_BV(1));
	((PIN(DA2_PORT) >> DA2_PIN) & 1) ? (retVal |= _BV(2)) : (retVal &= ~_BV(2));
	((PIN(DA3_PORT) >> DA3_PIN) & 1) ? (retVal |= _BV(3)) : (retVal &= ~_BV(3));
	((PIN(DA4_PORT) >> DA4_PIN) & 1) ? (retVal |= _BV(4)) : (retVal &= ~_BV(4));
	((PIN(DA5_PORT) >> DA5_PIN) & 1) ? (retVal |= _BV(5)) : (retVal &= ~_BV(5));
	((PIN(DA6_PORT) >> DA6_PIN) & 1) ? (retVal |= _BV(6)) : (retVal &= ~_BV(6));
	((PIN(DA7_PORT) >> DA7_PIN) & 1) ? (retVal |= _BV(7)) : (retVal &= ~_BV(7));
*/
#endif
	return retVal;
}



uint8_t read_psg_register(uint8_t psg_reg){
	uint8_t retVal = 0;
	// we can somehow mark that PSG is inactive so first inactive is not
	// necesarry
	set_psg_inactive();
	write_data_port(psg_reg);
	set_da_output();
	psg_latch();
	set_psg_inactive();
	set_da_input();
	set_psg_read();
	retVal = read_data_port();
	set_psg_inactive();
	return retVal;
}



void write_psg_register(uint8_t psg_reg,uint8_t val){
	// we can somehow mark that PSG is inactive so first inactive is not
	// necesarry
	set_psg_inactive();
	write_data_port(psg_reg);
	set_da_output();
	psg_latch();
	set_psg_inactive();
	write_data_port(val);
	set_psg_write();
	set_psg_inactive();
	set_da_input();
}

void psg_reset(){
		// set low
		PSG_RESET_PORT &= ~_BV(PSG_RESET_PIN);
		_delay_ms(1);
		// set high
		PSG_RESET_PORT |= _BV(PSG_RESET_PIN);
		_delay_ms(1);
}

void ay3_init(){
	// set out control port pins
	DDR(PSG_B_PORT)|=_BV(PSG_BDIR_PIN);
	DDR(PSG_B_PORT)|=_BV(PSG_BC1_PIN);
	// set out reset port pin
	DDR(PSG_RESET_PORT)|=_BV(PSG_RESET_PIN);
	set_psg_inactive();
	// set data address port in input mode
	set_da_input();
	_delay_ms(1);
	psg_reset();

}
