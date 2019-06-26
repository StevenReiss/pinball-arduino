/********************************************************************************/
/*										*/
/*	General constants for arduion pinball machine				*/
/*										*/
/********************************************************************************/



/********************************************************************************/
/*										*/
/*	Pin offsets for the various components					*/
/*										*/
/********************************************************************************/

// Save pins 0 and 1 for serial output while debugging
const int	RESERVE_START = 0;

// Pins 2 - 10, 16-17 are used for specials
const int	SPECIAL_PIN_START = 2;
const int	SPECIAL_PIN_START2 = 16;
const int	NUM_SPECIAL_PINS = 6 + 3 + 1 + 1;

// Pins 18 and 19 are not used

// Pins 11, 12, 13 are reserver for audio output
const int	SOUND_PLAY_PIN_START = 11;
const int	NUM_SOUND_PLAY_PINS = 3;

// Pins 14,15 are reserved for WIFI on Serial3

// Pin 20 is used for soft reset
const int	SOFT_RESET_PIN = 20;

// Pin 21 is the test switch -- starts test program rather than pinball game
const int	TEST_PIN = 21;

// Pins 22, 24, 26, 28, 30 are used for solenoids
const int	SOLENOID_PIN_START = 22;
const int	NUM_SOLENOID_PINS = 4 + 1;

// Pins 34, 36, 38, 40, 42 are used to drive existing sound card
const int	SOUND_PIN_START = 44;
const int	NUM_SOUND_PINS = 1 + 3 + 1;

// Pins 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53 (12 total) are for lights
//     not these are on both sides after 31
const int	LIGHT_PIN_START = 31;
const int	NUM_LIGHT_PINS = 3 + 1 + 8;

// Pin 23, 25, 27, 29 are not used

// Pins A0 .. A10 are used for switches
const int	SWITCH_PIN_START = A0;
const int	NUM_SWITCH_PINS = 8 + 3;

// Pins A11 .. A15 and 46, 48, 50, 52 are used for the display
const int	DISPLAY_PIN_START = A11;
const int	DISPLAY_PIN_START2 = 46;
const int	NUM_DISPLAY_PINS = 1 + 1 + 1 + 3 + 3;





/********************************************************************************/
/*										*/
/*	Other constants 							*/
/*										*/
/********************************************************************************/

const unsigned long MAX_TIME = 4294967295;

typedef void (*SwitchCallback)(int);

const long TEST_SWITCH_CHECK_TIME = 100000;

const int TEST_DOWN_CYCLES = 2;
const int RESET_DOWN_CYCLES = 10;



/********************************************************************************/
/*										*/
/*	External methods							*/
/*										*/
/********************************************************************************/

extern unsigned long addTime(unsigned long,unsigned long);




/* end of pinballdriver.h */
