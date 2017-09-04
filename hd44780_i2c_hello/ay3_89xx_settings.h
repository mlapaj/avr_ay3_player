#ifndef AY3_89XX_SETTINGS_H
#define AY3_89XX_SETTINGS_H

/*#define USE_ONE_DATA_PORT*/

#define PSG_B_PORT          PORTC // we must chose one control port
#define PSG_BC1_PIN               0     // BC1 and BDIR pins must be on same port
#define PSG_BDIR_PIN              1     // BDIR pin


#define PSG_RESET_PORT               PORTC
#define PSG_RESET_PIN                2

#ifdef USE_ONE_DATA_PORT
#define DA_PORT               PORTD
#else

#endif

#define DA0_PORT             PORTD
#define DA0_PIN              2
#define DA1_PORT             PORTD
#define DA1_PIN              3
#define DA2_PORT             PORTD
#define DA2_PIN              4
#define DA3_PORT             PORTD
#define DA3_PIN              5
#define DA4_PORT             PORTD
#define DA4_PIN              6
#define DA5_PORT             PORTD
#define DA5_PIN              7
#define DA6_PORT             PORTB
#define DA6_PIN              0
#define DA7_PORT             PORTB
#define DA7_PIN              1

#endif
