/*****************************************************************************
Title  :   HD44780 Library
Author :   SA Development
Version:   1.11
*****************************************************************************/

#include "avr/pgmspace.h"
#include "hd44780.h"
#include "avr/sfr_defs.h"
#include "i2c.h"

// bit configuration
// 0st bit - RS
// 1st bit - RW
// 2nd bit - E
// 3rd bit - KATODA
// 4th bit - D4
// 5th bit - D5
// 6th bit - D6
// 7th bit - D7

uint8_t out_port = 0;
#define MUX_ADDR 0x3F



void lcd_rs_port_low(){
	out_port &= ~_BV(0);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_rs_port_high(){
	out_port |= _BV(0);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_rw_port_low(){
	out_port &= ~_BV(1);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_rw_port_high(){
	out_port |= _BV(1);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_e_port_low(){
	out_port &= ~_BV(2);
	i2c_write(MUX_ADDR,out_port);
}

void lcd_e_port_high(){
	out_port |= _BV(2);
	i2c_write(MUX_ADDR,out_port);
}


void backlight_off(){
	out_port &= ~_BV(3);
	i2c_write(MUX_ADDR,out_port);
}
void backlight_on(){
	out_port |= _BV(3);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_db4_port_low(){
	out_port &= ~_BV(4);
	i2c_write(MUX_ADDR,out_port);
}

void lcd_db4_port_high(){
	out_port |= _BV(4);
	i2c_write(MUX_ADDR,out_port);
}

void lcd_db5_port_low(){
	out_port &= ~_BV(5);
	i2c_write(MUX_ADDR,out_port);
}

void lcd_db5_port_high(){
	out_port |= _BV(5);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_db6_port_low(){
	out_port &= ~_BV(6);
	i2c_write(MUX_ADDR,out_port);
}

void lcd_db6_port_high(){
	out_port |= _BV(6);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_db7_port_low(){
	out_port &= ~_BV(7);
	i2c_write(MUX_ADDR,out_port);
}


void lcd_db7_port_high(){
	out_port |= _BV(7);
	i2c_write(MUX_ADDR,out_port);
}


int lcd_db4_pin_get(){
	uint8_t retVal = 0;
	lcd_db4_port_low();
	i2c_read(MUX_ADDR,&retVal);
	return (retVal >> 4) & 1;
}


int lcd_db5_pin_get(){
	uint8_t retVal = 0;
	lcd_db5_port_low();
	i2c_read(MUX_ADDR,&retVal);
	return (retVal >> 5) & 1;
}


int lcd_db6_pin_get(){
	uint8_t retVal = 0;
	lcd_db6_port_low();
	i2c_read(MUX_ADDR,&retVal);
	return (retVal >> 6) & 1;
}


int lcd_db7_pin_get(){
	uint8_t retVal = 0;
	lcd_db7_port_low();
	i2c_read(MUX_ADDR,&retVal);
	return (retVal >> 7) & 1;
}

#define lcd_db7_ddr_low lcd_db7_port_low
#define lcd_db6_ddr_low lcd_db6_port_low
#define lcd_db5_ddr_low lcd_db5_port_low
#define lcd_db4_ddr_low lcd_db4_port_low

#define lcd_db7_ddr_high(...) //i2c_write(MUX_ADDR,out_port);
#define lcd_db6_ddr_high(...) //i2c_write(MUX_ADDR,out_port);
#define lcd_db5_ddr_high(...) //i2c_write(MUX_ADDR,out_port);
#define lcd_db4_ddr_high(...) //i2c_write(MUX_ADDR,out_port);

#define lcd_db7_port_set(x) { (x==0) ? lcd_db7_port_low() : lcd_db7_port_high(); }
#define lcd_db6_port_set(x) { (x==0) ? lcd_db6_port_low() : lcd_db6_port_high(); }
#define lcd_db5_port_set(x) { (x==0) ? lcd_db5_port_low() : lcd_db5_port_high(); }
#define lcd_db4_port_set(x) { (x==0) ? lcd_db4_port_low() : lcd_db4_port_high(); }

#if (USE_ADELAY_LIBRARY==1)
  #include "adelay.h"
#else
  #define Delay_ns(__ns) \
    if((unsigned long) (F_CPU/1000000000.0 * __ns) != F_CPU/1000000000.0 * __ns)\
          __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns)+1);\
    else __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns))
  #define Delay_us(__us) \
    if((unsigned long) (F_CPU/1000000.0 * __us) != F_CPU/1000000.0 * __us)\
          __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000.0 * __us)+1);\
    else __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000000.0 * __us))
  #define Delay_ms(__ms) \
    if((unsigned long) (F_CPU/1000.0 * __ms) != F_CPU/1000.0 * __ms)\
          __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000.0 * __ms)+1);\
    else __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1000.0 * __ms))
  #define Delay_s(__s) \
    if((unsigned long) (F_CPU/1.0 * __s) != F_CPU/1.0 * __s)\
          __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1.0 * __s)+1);\
    else __builtin_avr_delay_cycles((unsigned long) ( F_CPU/1.0 * __s))
#endif



#if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
static unsigned char PrevCmdInvolvedAddressCounter=0;
#endif

#if (LCD_DISPLAYS>1)
static unsigned char ActiveDisplay=1;
#endif



void flash_backlight(){
	  Delay_ms(500);
      backlight_off();
	  Delay_ms(500);
	  backlight_on();
}



/*************************************************************************
loops while lcd is busy, returns address counter
*************************************************************************/
#if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
static uint8_t lcd_read(uint8_t rs);


static void lcd_waitbusy(void)
  {
    register uint8_t c;
    unsigned int ul1=0;

    while ( ((c=lcd_read(0)) & (1<<LCD_BUSY)) && ul1<((F_CPU/16384>=16)?F_CPU/16384:16))     // Wait Until Busy Flag is Cleared
      ul1++;
  }
#endif


/*************************************************************************
Low-level function to read byte from LCD controller
Input:    rs     1: read data
                 0: read busy flag / address counter
Returns:  byte read from LCD controller
*************************************************************************/
#if RW_LINE_IMPLEMENTED==1
static uint8_t lcd_read(uint8_t rs)
  {
    uint8_t data;
    
    #if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
    if (rs)
      lcd_waitbusy();
      if (PrevCmdInvolvedAddressCounter)
        {
          Delay_us(5);
          PrevCmdInvolvedAddressCounter=0;
        }
    #endif

    if (rs)
      {
        lcd_rs_port_high();                             // RS=1: Read Data
        #if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
        PrevCmdInvolvedAddressCounter=1;
        #endif
      }
    else lcd_rs_port_low();                           // RS=0: Read Busy Flag

    
    lcd_rw_port_high();                               // RW=1: Read Mode

    #if LCD_BITS==4
      lcd_db7_ddr_low();                              // Configure Data Pins as Input
      lcd_db6_ddr_low();
      lcd_db5_ddr_low();
      lcd_db4_ddr_low();

      lcd_e_port_high();                              // Read High Nibble First
      Delay_ns(500);

      data=lcd_db4_pin_get() << 4 | lcd_db5_pin_get() << 5 |
           lcd_db6_pin_get() << 6 | lcd_db7_pin_get() << 7;

      lcd_e_port_low();
      Delay_ns(500);

      lcd_e_port_high();                              // Read Low Nibble
      Delay_ns(500);

      data|=lcd_db4_pin_get() << 0 | lcd_db5_pin_get() << 1 |
            lcd_db6_pin_get() << 2 | lcd_db7_pin_get() << 3;

      lcd_e_port_low();

      lcd_db7_ddr_high();                             // Configure Data Pins as Output
      lcd_db6_ddr_high();
      lcd_db5_ddr_high();
      lcd_db4_ddr_high();

      lcd_db7_port_high();                            // Pins High (Inactive)
      lcd_db6_port_high();
      lcd_db5_port_high();
      lcd_db4_port_high();
    #else //using 8-Bit-Mode
      lcd_db7_ddr_low();                              // Configure Data Pins as Input
      lcd_db6_ddr_low();
      lcd_db5_ddr_low();
      lcd_db4_ddr_low();
      lcd_db3_ddr_low();
      lcd_db2_ddr_low();
      lcd_db1_ddr_low();
      lcd_db0_ddr_low();

      lcd_e_port_high();
      Delay_ns(500);

      data=lcd_db7_pin_get() << 7 | lcd_db6_pin_get() << 6 |
           lcd_db5_pin_get() << 5 | lcd_db4_pin_get() << 4 |
           lcd_db3_pin_get() << 3 | lcd_db2_pin_get() << 2 |
           lcd_db1_pin_get() << 1 | lcd_db0_pin_get();

      lcd_e_port_low();

      lcd_db7_ddr_high();                             // Configure Data Pins as Output
      lcd_db6_ddr_high();
      lcd_db5_ddr_high();
      lcd_db4_ddr_high();
      lcd_db3_ddr_high();
      lcd_db2_ddr_high();
      lcd_db1_ddr_high();
      lcd_db0_ddr_high();

      lcd_db7_port_high();                            // Pins High (Inactive)
      lcd_db6_port_high();
      lcd_db5_port_high();
      lcd_db4_port_high();
      lcd_db3_port_high();
      lcd_db2_port_high();
      lcd_db1_port_high();
      lcd_db0_port_high();
    #endif
    
    lcd_rw_port_low();

    #if (WAIT_MODE==0 || RW_LINE_IMPLEMENTED==0)
    if (rs)
      Delay_us(40);
    else Delay_us(1);
    #endif
    return data;
  }


uint8_t lcd_getc()
  {
    return lcd_read(1);
  }

#endif


/*************************************************************************
Low-level function to write byte to LCD controller
Input:    data   byte to write to LCD
          rs     1: write data
                 0: write instruction
Returns:  none
*************************************************************************/
static void lcd_write(uint8_t data,uint8_t rs)
  {
    #if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
      lcd_waitbusy();
      if (PrevCmdInvolvedAddressCounter)
        {
          Delay_us(5);
          PrevCmdInvolvedAddressCounter=0;
        }
    #endif

    if (rs)
      {
        lcd_rs_port_high();                            // RS=1: Write Character
        #if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
        PrevCmdInvolvedAddressCounter=1;
        #endif
      }
    else
      {
        lcd_rs_port_low();                          // RS=0: Write Command
        #if (WAIT_MODE==1 && RW_LINE_IMPLEMENTED==1)
        PrevCmdInvolvedAddressCounter=0;
        #endif
      }

	lcd_db7_port_set(data&_BV(7));                  //Output High Nibble
	lcd_db6_port_set(data&_BV(6));
	lcd_db5_port_set(data&_BV(5));
	lcd_db4_port_set(data&_BV(4));

	Delay_ns(100);
	lcd_e_port_high();

	Delay_ns(500);
	lcd_e_port_low();

	lcd_db7_port_set(data&_BV(3));                  //Output High Nibble
	lcd_db6_port_set(data&_BV(2));
	lcd_db5_port_set(data&_BV(1));
	lcd_db4_port_set(data&_BV(0));

	Delay_ns(100);
	lcd_e_port_high();

	Delay_ns(500);
	lcd_e_port_low();

	lcd_db7_port_high();                            // All Data Pins High (Inactive)
	lcd_db6_port_high();
	lcd_db5_port_high();
	lcd_db4_port_high();


#if (WAIT_MODE==0 || RW_LINE_IMPLEMENTED==0)
	if (!rs && data<=((1<<LCD_CLR) | (1<<LCD_HOME))) // Is command clrscr or home?
		Delay_us(1640);
	else Delay_us(40);
#endif
  }

/*************************************************************************
Send LCD controller instruction command
Input:   instruction to send to LCD controller, see HD44780 data sheet
Returns: none
*************************************************************************/
void lcd_command(uint8_t cmd)
  {
    lcd_write(cmd,0);
  }

/*************************************************************************
Set cursor to specified position
Input:    pos position
Returns:  none
*************************************************************************/
void lcd_goto(uint8_t pos)
  {
    lcd_command((1<<LCD_DDRAM)+pos);
  }


/*************************************************************************
Clear screen
Input:    none
Returns:  none
*************************************************************************/
void lcd_clrscr()
  {
    lcd_command(1<<LCD_CLR);
  }


/*************************************************************************
Return home
Input:    none
Returns:  none
*************************************************************************/
void lcd_home()
  {
    lcd_command(1<<LCD_HOME);
  }


/*************************************************************************
Display character
Input:    character to be displayed
Returns:  none
*************************************************************************/
void lcd_putc(char c)
  {
    lcd_write(c,1);
  }


/*************************************************************************
Display string
Input:    string to be displayed
Returns:  none
*************************************************************************/
void lcd_puts(const char *s)
  {
    register char c;

    while ((c=*s++)) 
      lcd_putc(c);
	flash_backlight();
  }


/*************************************************************************
Display string from flash
Input:    string to be displayed
Returns:  none
*************************************************************************/
void lcd_puts_P(const char *progmem_s)
  {
    register char c;

    while ((c=pgm_read_byte(progmem_s++))) 
      lcd_putc(c);
  }


/*************************************************************************
Initialize display
Input:    none
Returns:  none
*************************************************************************/
void lcd_init()
  {
    backlight_on();
    lcd_e_port_low();
    lcd_rs_port_low();
	lcd_db5_port_high();
	lcd_db4_port_high();
	Delay_ns(100);
    lcd_e_port_high();
    Delay_ns(500);
    lcd_e_port_low();

	lcd_e_port_high();
    Delay_ns(500);
    lcd_e_port_low();



    //Display Clear
    lcd_clrscr();

    lcd_command(_BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_CURSOR));


	return;

    //Set All Control Lines Low
    lcd_e_port_low();
    lcd_rs_port_low();
	#if RW_LINE_IMPLEMENTED==1
      lcd_rw_port_low();
    #endif
    //Set All Data Lines High
    lcd_db7_port_high();
    lcd_db6_port_high();
    lcd_db5_port_high();
    lcd_db4_port_high();

    //Startup Delay
    Delay_ms(DELAY_RESET);

    //Initialize Display
    lcd_db7_port_low();
    lcd_db6_port_low();
    Delay_ns(100);
    lcd_e_port_high();
    Delay_ns(500);
    lcd_e_port_low();

    Delay_us(4100);

    lcd_e_port_high();
    Delay_ns(500);
    lcd_e_port_low();

    Delay_us(100);

    lcd_e_port_high();
    Delay_ns(500);
    lcd_e_port_low();

    Delay_us(40);

	//Init differs between 4-bit and 8-bit from here
	lcd_db4_port_low();
	Delay_ns(100);
	lcd_e_port_high();
	Delay_ns(500);
	lcd_e_port_low();
	Delay_us(40);

	lcd_db4_port_low();
	Delay_ns(100);
	lcd_e_port_high();
	Delay_ns(500);
	lcd_e_port_low();
	Delay_ns(500);

#if (LCD_DISPLAYS==1)
	if (LCD_DISPLAY_LINES>1)
		lcd_db7_port_high();
#else
	unsigned char c;
	switch (ActiveDisplay)
	{
		case 1 : c=LCD_DISPLAY_LINES; break;
		case 2 : c=LCD_DISPLAY2_LINES; break;
#if (LCD_DISPLAYS>=3)
		case 3 : c=LCD_DISPLAY3_LINES; break;
#endif
#if (LCD_DISPLAYS==4)
		case 4 : c=LCD_DISPLAY4_LINES; break;
#endif
	}
	if (c>1)
		lcd_db7_port_high();
#endif

	Delay_ns(100);
      lcd_e_port_high();
      Delay_ns(500);
      lcd_e_port_low();
      Delay_us(40);



    //Display Off
    lcd_command(_BV(LCD_DISPLAYMODE));

    //Display Clear
    lcd_clrscr();

    //Entry Mode Set
    lcd_command(_BV(LCD_ENTRY_MODE) | _BV(LCD_ENTRY_INC));

    //Display On
    lcd_command(_BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_CURSOR) | _BV(LCD_DISPLAYMODE_BLINK)| _BV(LCD_DISPLAYMODE_ON));
  }

#if (LCD_DISPLAYS>1)
void lcd_use_display(int ADisplay)
  {
    if (ADisplay>=1 && ADisplay<=LCD_DISPLAYS)
      ActiveDisplay=ADisplay;
  }
#endif

