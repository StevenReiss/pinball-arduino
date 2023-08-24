/********************************************************************************/
/*										*/
/*	switches.h -- definitions for switches					*/
/*										*/
/********************************************************************************/



/********************************************************************************/
/*										*/
/*	Pin Assignments 							*/
/*										*/
/********************************************************************************/

const int SWITCH_PIN_READ0 = SWITCH_PIN_START + 0;
const int SWITCH_PIN_READ1 = SWITCH_PIN_START + 1;
const int SWITCH_PIN_READ2 = SWITCH_PIN_START + 2;
const int SWITCH_PIN_READ3 = SWITCH_PIN_START + 3;
const int SWITCH_PIN_READ4 = SWITCH_PIN_START + 4;
const int SWITCH_PIN_READ5 = SWITCH_PIN_START + 5;
const int SWITCH_PIN_READ6 = SWITCH_PIN_START + 6;
const int SWITCH_PIN_READ7 = SWITCH_PIN_START + 7;

const int SWITCH_PIN_SELECT0 = SWITCH_PIN_START + 8;
const int SWITCH_PIN_SELECT1 = SWITCH_PIN_START + 9;
const int SWITCH_PIN_SELECT2 = SWITCH_PIN_START + 10;

const int SWITCH_OUTPUT_0 = SPARE_1;

#define SWITCH_PIN_READ(i)	(SWITCH_PIN_READ0 + i)





/********************************************************************************/
/*										*/
/*	Swtich constants							*/
/*										*/
/********************************************************************************/

const int	NUM_SWITCH_ROWS = 8;
const int	NUM_SWITCH_COLS = 8;
const int	NUM_SWITCH = NUM_SWITCH_ROWS*NUM_SWITCH_COLS;

const long	SWITCH_CHECK_TIME = 2000l;
const long	SWITCH_DELAY_TIME = 1;		// non-zero to read later
const int	SWITCH_BOUNCE_CYCLES = 1;	// number of times before report

const int	SWITCH_ON = LOW;



/********************************************************************************/
/*										*/
/*	Entry points								*/
/*										*/
/********************************************************************************/

extern bool getSwitch(int);
extern void handleSwitchChanges(int,SwitchCallback,SwitchCallback);

extern void switchesSetup();
extern void switchesWrap();
extern void switchesUpdate(unsigned long);
extern void switchesReset();



/* end of switches.h */

