/********************************************************************************/
/*										*/
/*	lights.h -- definitions for the lights					*/
/*										*/
/********************************************************************************/


/********************************************************************************/
/*										*/
/*	Pin assignments 							*/
/*										*/
/********************************************************************************/

const int LIGHT_PIN_SELECT0 = LIGHT_PIN_START + 22;
const int LIGHT_PIN_SELECT1 = LIGHT_PIN_START + 20;
const int LIGHT_PIN_SELECT2 = LIGHT_PIN_START + 18;
const int LIGHT_PIN_ENABLE = LIGHT_PIN_START + 16;

const int LIGHT_PIN_DRIVE0 = LIGHT_PIN_START + 14;
const int LIGHT_PIN_DRIVE1 = LIGHT_PIN_START + 12;
const int LIGHT_PIN_DRIVE2 = LIGHT_PIN_START + 10;
const int LIGHT_PIN_DRIVE3 = LIGHT_PIN_START + 8;
const int LIGHT_PIN_DRIVE4 = LIGHT_PIN_START + 6;
const int LIGHT_PIN_DRIVE5 = LIGHT_PIN_START + 4;
const int LIGHT_PIN_DRIVE6 = LIGHT_PIN_START + 2;
const int LIGHT_PIN_DRIVE7 = LIGHT_PIN_START + 0;

#define LIGHT_PIN_DRIVE(i) (LIGHT_PIN_DRIVE0 - i*2)




/********************************************************************************/
/*										*/
/*	Timing values								*/
/*										*/
/********************************************************************************/

const int	NUM_LIGHT_ROWS = 8;
const int	NUM_LIGHT_COLUMNS = 8;
const int	NUM_LIGHTS = 64;

const int	LIGHT_NUM_ENABLED_ROW = 3;

const long	LIGHT_PULSE_TIME = 1000;
const long	LIGHT_CYCLE_TIME = 150000;	   // 8/3 * this yields blink time

//const long  LIGHT_PULSE_TIME = 5000000;
//const long  LIGHT_CYCLE_TIME = 150000;     // 8/3 * this yields blink time

const int	LIGHT_DRIVE_ON = HIGH;
const int	LIGHT_DRIVE_OFF = LOW;



/********************************************************************************/
/*										*/
/*	entry points								*/
/*										*/
/********************************************************************************/

extern void setLight(int,bool);
extern void lightOn(int);
extern void lightOff(int);
extern void disableAllLights();
extern bool isLightEnabled(int);

extern void lightsSetup();
extern void lightsWrap();
extern void lightsUpdate(unsigned long);




/* end of lights.h */
