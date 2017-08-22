#ifndef AY3_89XX_SETTINGS_H
#define AY3_89XX_SETTINGS_H

#define F_CPU                    1000000L     // Set Clock Frequency

#define USE_ONE_DATA_PORT

#define PSG_B_PORT          PORTB // we must chose one control port
#define PSG_BC1_PIN               2     // BC1 and BDIR pins must be on same port
#define PSG_BDIR_PIN              1     // BDIR pin


#define PSG_RESET_PORT               PORTB
#define PSG_RESET_PIN                7

#ifdef USE_ONE_DATA_PORT
#define DA_PORT               PORTD
#else

#endif

#define DA0_PORT             PORTD
#define DA0_PIN              0
#define DA1_PORT             PORTD
#define DA1_PIN              1
#define DA2_PORT             PORTD
#define DA2_PIN              2
#define DA3_PORT             PORTD
#define DA3_PIN              3
#define DA4_PORT             PORTD
#define DA4_PIN              4
#define DA5_PORT             PORTD
#define DA5_PIN              5
#define DA6_PORT             PORTD
#define DA6_PIN              6
#define DA7_PORT             PORTD
#define DA7_PIN              7

#endif
