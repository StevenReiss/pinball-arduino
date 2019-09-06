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
const int	SOUND_PIN_START = 34;
const int	NUM_SOUND_PINS = 1 + 3 + 1;

// Pins 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53 (12 total) are for lights
//     not these are on both sides after 31
const int	LIGHT_PIN_START = 31;
const int	NUM_LIGHT_PINS = 3 + 1 + 8;

// Pins 23, 25, 27 are used for test switches
const int	TEST_PIN_START = 23;

// Pin 29 is not used

// Pins A0 .. A10 are used for switches
const int	SWITCH_PIN_START = A0;
const int	NUM_SWITCH_PINS = 8 + 3;

// Pins A11 .. A15 and 44, 46, 48, 50, 52 are used for the display
const int	DISPLAY_PIN_START = A11;
const int	DISPLAY_PIN_START2 = 44;
const int	NUM_DISPLAY_PINS = 2 + 1 + 1 + 3 + 3;





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


/********************************************************************************/
/*										*/
/*	Definitions for handliogn reset 					*/
/*										*/
/********************************************************************************/

enum rst_reason {
   REASON_DEFAULT_RST		   = 0,
   REASON_WDT_RST		   = 1,
   REASON_EXCEPTION_RST    = 2,
   REASON_SOFT_WDT_RST	   = 3,
   REASON_SOFT_RESTART	   = 4,
   REASON_DEEP_SLEEP_AWAKE = 5,
   REASON_EXT_SYS_RST	   = 6
};

typedef unsigned long uint32;
typedef byte uint8;
typedef unsigned short uint16;

struct rst_info{
   uint32 reason;
   uint32 exccause;
   uint32 epc1;
   uint32 epc2;
   uint32 epc3;
   uint32 excvaddr;
   uint32 depc;
};




/* end of pinballdriver.h */
