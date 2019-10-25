/********************************************************************************/
/*										*/
/*	Constants for Testing							*/
/*										*/
/********************************************************************************/

enum TestMode {
   TEST_IDLE, TEST_SOLENOIDS, TEST_SOUNDS, TEST_LIGHTS, TEST_SPECIALS
};


const unsigned long TEST_CHECK_TIME = 10000;

const unsigned long TEST_START_INTERVAL = 5000000;

const unsigned long TEST_SOLENOID_INTERVAL = 5000000;

const unsigned long TEST_SOUND_INTERVAL = 10000000;

const unsigned long TEST_LIGHT_INTERVAL = 5000000;

const unsigned long TEST_DISPLAY_COUNT_INTERVAL = 500000;

const unsigned long TEST_SPECIAL_INTERVAL = 5000000;


const int TEST_BOUNCE = 2;



/********************************************************************************/
/*										*/
/*	Pin assignments for Testing						*/
/*										*/
/********************************************************************************/

const int	TEST_PIN_BUTTON = TEST_PIN;
const int	TEST_PIN_MEM_PROTECT = TEST_PIN_START;
const int	TEST_PIN_AUTO_MANUAL = TEST_PIN_START + 2;
const int	TEST_PIN_ADVANCE = TEST_PIN_START + 4;



/********************************************************************************/
/*										*/
/*	Timing structures							*/
/*										*/
/********************************************************************************/

struct DeltaTimer {
   char * time_name;
   unsigned long time_last;
   long time_count;
   long time_total;
   long time_max;
   int time_reports;
};

typedef struct DeltaTimer * DeltaTimerP;



/* end of testing.h */
