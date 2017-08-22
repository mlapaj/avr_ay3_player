#ifndef HD44780_SETTINGS_H
#define HD44780_SETTINGS_H

#define F_CPU                    1000000L     // Set Clock Frequency

#define USE_ADELAY_LIBRARY       0           // Set to 1 to use my ADELAY library, 0 to use internal delay functions
#define LCD_BITS                 4           // 4 for 4 Bit I/O Mode, 8 for 8 Bit I/O Mode
#define RW_LINE_IMPLEMENTED      1           // 0 for no RW line (RW on LCD tied to ground), 1 for RW line present
#define WAIT_MODE                0           // 0=Use Delay Method (Faster if running <10Mhz)
                                             // 1=Use Check Busy Flag (Faster if running >10Mhz) ***Requires RW Line***
#define DELAY_RESET              15          // in mS

#define LCD_DB4_PORT             PORTC       // If using 4 bit omde, yo umust configure DB4-DB7
#define LCD_DB4_PIN              0
#define LCD_DB5_PORT             PORTC
#define LCD_DB5_PIN              1
#define LCD_DB6_PORT             PORTC
#define LCD_DB6_PIN              2
#define LCD_DB7_PORT             PORTC
#define LCD_DB7_PIN              3

#define LCD_RS_PORT              PORTC       // Port for RS line
#define LCD_RS_PIN               4           // Pin for RS line

#define LCD_RW_PORT              PORTC       // Port for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)
#define LCD_RW_PIN               5           // Pin for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)

#define LCD_DISPLAYS             1           // Up to 4 LCD displays can be used at one time
                                             // All pins are shared between displays except for the E
                                             // pin which each display will have its own

                                             // Display 1 Settings - if you only have 1 display, YOU MUST SET THESE
#define LCD_DISPLAY_LINES        2           // Number of Lines, Only Used for Set I/O Mode Command
#define LCD_E_PORT               PORTB       // Port for E line
#define LCD_E_PIN                0           // Pin for E line


#endif

