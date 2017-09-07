#ifndef pffArduino_h
#define pffArduino_h
#include <avr/io.h>
#include <util/delay_basic.h>
#include "integer.h"

// SD chip select pin
#define SD_CS_PORT PORTB
#define SD_CS_DDR DDRB
#define SD_CS_BIT 2

// Use SPI SCK divisor of 2 if nonzero else 4.
#define SPI_FCPU_DIV_2 0
//------------------------------------------------------------------------------
#define	FORWARD(d)	xmit(d)				/* Data forwarding function (console out) */
static void xmit(char d) {}  // Dummy write console
//------------------------------------------------------------------------------
static void spi_set_divisor(BYTE cardType) {
  if (!cardType) {
    // Set slow speed for initialization.
    SPCR = (1 << SPE) | (1 << MSTR) | 3;
    SPSR = 0;
  }  else {
    // Set high speed.
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = SPI_FCPU_DIV_2 ? 1 << SPI2X : 0;
  }
}
//------------------------------------------------------------------------------
/** Send a byte to the card */
inline void xmit_spi(BYTE d) {SPDR = d; while(!(SPSR & (1 << SPIF)));}
//------------------------------------------------------------------------------
/** Receive a byte from the card */
inline BYTE rcv_spi (void) {xmit_spi(0XFF); return SPDR;}
//------------------------------------------------------------------------------
// Optimize 168 and 328 Arduinos.
#define SD_CS_MASK (1 << SD_CS_BIT)
#define SELECT()  (SD_CS_PORT &= ~SD_CS_MASK)	 /* CS = L */
#define	DESELECT()	(SD_CS_PORT |= SD_CS_MASK)	/* CS = H */
#define	SELECTING	!(SD_CS_PORT & SD_CS_MASK)	  /* CS status (true:CS low) */

static void init_spi (void) {
  // Save a few bytes for 328 CPU - gcc optimizes single bit '|' to sbi.
  PORTB |= 1 << 2;  // SS high
  DDRB  |= 1 << 2;  // SS output mode
  DDRB  |= 1 << 3;  // MOSI output mode
  DDRB  |= 1 << 5;  // SCK output mode
  SD_CS_DDR |= SD_CS_MASK;
  spi_set_divisor(0);
}
//------------------------------------------------------------------------------
static void dly_100us (void) {
  // each count delays four CPU cycles.
  _delay_loop_2(F_CPU/(40000));
}
#endif  // pffArduino_h
