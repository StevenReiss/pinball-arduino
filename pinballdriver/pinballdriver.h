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

// Use pins 2 through 10 for display
const int	DISPLAY_PIN_START = 2;

// Pins 11 12 and 13 are reserver for audio output
const int	SOUND_PLAY_PIN_START = 11;	

// Pins 14,15 are reserved for WIFI on Serial3

// Pins 16 through 21 and { 22, 24, 26, 28, 30, 32 } (on top of bottom row) are used for light control
const int	LIGHT_PIN_START = 16;
const int       LIGHT_PIN_START2 = 22;

// Pins A0 to A4 are used for solenoids
const int       SOLENOID_PIN_START = A0;

// Pin A5 is not used

// Pins A6 to A12 and 44,46,48,50,52,54, are used for special
const int       SPECIAL_PIN_START = A5;
const int       SPECIAL_PIN_START2 = 44;

// Pins A13 is not used

// Pin A14 is the test switch -- starts test program rather than pinball game
const int       TEST_PIN = A14;

// Pins 23,25,...43 are used for switches
const int	SWITCH_PIN_START = 23;

// Pins 45,47,49,51,53 are used for driving existing sound card
const int	SOUND_PIN_START = 45;


// Pins 34,36,38,40,42 are unused




/********************************************************************************/
/*										*/
/*	Other constants 							*/
/*										*/
/********************************************************************************/

const unsigned long MAX_TIME = 4294967295;

typedef void (*SwitchCallback)(int);


/********************************************************************************/
/*										*/
/*	External methods							*/
/*										*/
/********************************************************************************/

extern unsigned long addTime(unsigned long,unsigned long);




/* end of pinballdriver.h */
