#include "i2c.h"
#include <util/twi.h>
#include <avr/power.h>
#include <util/delay.h>



int i2c_read(uint8_t addr,uint8_t *data){
	TWCR = (1<<TWINT)| (1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_START) goto error;
	TWDR = (addr << 1) | 1;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_MR_SLA_ACK) goto error;
	TWCR= (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1<<TWINT))); // 1x00x10x
	if ((TWSR & 0xF8) != TW_MR_DATA_NACK) goto error;
	*data = TWDR;
	TWCR = (1<<TWINT)| (1<<TWSTO);
	return 0;
error:
	return 1;
}


int i2c_write(uint8_t addr,uint8_t data){

	TWCR = (1<<TWINT)| (1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_START) goto error;
	TWDR = (addr << 1);
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) goto error;
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) goto error;
	TWCR = (1<<TWINT)| (1<<TWSTO);
	return 0;
error:
	return 1;
}

void i2c_init(){
	power_twi_enable();
	TWCR = (1<<TWEN);
	_delay_ms(100);
	TWBR = 72; // 16 Mhz = 100kHz * (16 + 2 * x * 1)
	TWSR = 0;
}
