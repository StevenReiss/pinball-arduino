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

// Pins 2 ... 9 and 14 ... 16 are used for switches
const int       SWITCH_PIN_START = 2;
const int       SWITCH_PIN_START2= 14;

// Pin 10 is unused

// Pins 11 12 and 13 are reserver for audio output
const int        SOUND_PLAY_PIN_START = 11;      

// Pins 14,15 are reserved for WIFI on Serial3


// Pins 17 .. 21 and 22,24,26,28 are used for the display
const int       DISPLAY_PIN_START = 17;
const int       DISPLAY_PIN_START2 = 22;

// Pins 23, 25, 27, 29, 31, 32, 33, 34, 35, 36, 37, 38 (12 total) are for lights
//     not these are on both sides after 31
const int       LIGHT_PIN_START = 23;
const int       LIGHT_PIN_START2 = 31;

// Pin 30 is not used

// Pin A0 is the test switch -- starts test program rather than pinball game
const int       TEST_PIN = A0;

// PIn A1 is the soft reset button
const int       SOFT_RESET_PIN = A1;

// Pins A2 to A14, are used for special
const int       SPECIAL_PIN_START = A2;

// Pin A15 is not used

// Pins 45, 47, 49, 51, 53 are used for solenoids
const int       SOLENOID_PIN_START = 45;

// Pins 39, 41, 43 are unused

// Pins 44,46,48,50,52 are used for driving existing sound card
const int	SOUND_PIN_START = 44;

// Pins 40, 42 are unused



/********************************************************************************/
/*										*/
/*	Other constants 							*/
/*										*/
/********************************************************************************/

const unsigned long MAX_TIME = 4294967295;

typedef void (*SwitchCallback)(int);

const long TEST_SWITCH_CHECK_TIME = 100000;


/********************************************************************************/
/*                                                                              */
/*      Hardward definitions for soft reset                                     */
/*                                                                              */
/********************************************************************************/

enum rst_reason {
        REASON_DEFAULT_RST              = 0,
        REASON_WDT_RST                  = 1,
        REASON_EXCEPTION_RST    = 2,
        REASON_SOFT_WDT_RST     = 3,
        REASON_SOFT_RESTART     = 4,
        REASON_DEEP_SLEEP_AWAKE = 5,
        REASON_EXT_SYS_RST      = 6
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



/********************************************************************************/
/*										*/
/*	External methods							*/
/*										*/
/********************************************************************************/

extern unsigned long addTime(unsigned long,unsigned long);




/* end of pinballdriver.h */
