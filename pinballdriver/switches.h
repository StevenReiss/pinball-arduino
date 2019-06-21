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

const int SWITCH_PIN_SELECT0 = SWITCH_PIN_START;
const int SWITCH_PIN_SELECT1 = SWITCH_PIN_START + 2;
const int SWITCH_PIN_SELECT2 = SWITCH_PIN_START + 4;

const int SWITCH_PIN_READ0 = SWITCH_PIN_START + 6;
const int SWITCH_PIN_READ1 = SWITCH_PIN_START + 8;
const int SWITCH_PIN_READ2 = SWITCH_PIN_START + 10;
const int SWITCH_PIN_READ3 = SWITCH_PIN_START + 12;
const int SWITCH_PIN_READ4 = SWITCH_PIN_START + 14;
const int SWITCH_PIN_READ5 = SWITCH_PIN_START + 16;
const int SWITCH_PIN_READ6 = SWITCH_PIN_START + 18;
const int SWITCH_PIN_READ7 = SWITCH_PIN_START + 20;

#define SWITCH_PIN_READ(i)      (SWITCH_PIN_READ0 + i*2)




/********************************************************************************/
/*										*/
/*	Swtich constants							*/
/*										*/
/********************************************************************************/

const int	NUM_SWITCH_ROWS = 8;
const int	NUM_SWITCH_COLS = 8;
const int	NUM_SWITCH = NUM_SWITCH_ROWS*NUM_SWITCH_COLS;

const long	SWITCH_CHECK_TIME = 5000;
const long	SWITCH_DELAY_TIME = 10; 	// delay after setting selectors before reading

const int	SWITCH_BOUNCE_CYCLES = 2;	// number of consistent times before report

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




/* end of switches.h */
