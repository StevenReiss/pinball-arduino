/********************************************************************************/
/*                                                                              */
/*      lights.h -- definitions for the lights                                  */      
/*                                                                              */
/********************************************************************************/


/********************************************************************************/
/*                                                                              */
/*      Pin assignments                                                         */      
/*                                                                              */
/********************************************************************************/

const int LIGHT_PIN_SELECT0 = LIGHT_PIN_START;
const int LIGHT_PIN_SELECT1 = LIGHT_PIN_START + 1;
const int LIGHT_PIN_SELECT2 = LIGHT_PIN_START + 2;
const int LIGHT_PIN_DRIVE0 = LIGHT_PIN_START + 3;
const int LIGHT_PIN_DRIVE1 = LIGHT_PIN_START + 4;
const int LIGHT_PIN_DRIVE2 = LIGHT_PIN_START + 5;
const int LIGHT_PIN_DRIVE3 = LIGHT_PIN_START2 + 0;
const int LIGHT_PIN_DRIVE4 = LIGHT_PIN_START2 + 2;;
const int LIGHT_PIN_DRIVE5 = LIGHT_PIN_START2 + 4;
const int LIGHT_PIN_DRIVE6 = LIGHT_PIN_START2 + 6;
const int LIGHT_PIN_DRIVE7 = LIGHT_PIN_START2 * 8;
const int LIGHT_PIN_SELECT = LIGHT_PIN_START2 + 10;

#define LIGHT_PIN_DRIVE(i) (i >= 3 ? LIGHT_PIN_DRIVE5 + (i-3)*2 : LIGHT_PIN_DRIVE0 + i)




/********************************************************************************/
/*                                                                              */
/*      Timing values                                                           */      
/*                                                                              */
/********************************************************************************/

const int       NUM_LIGHT_ROWS = 8;
const int       NUM_LIGHT_COLUMNS = 8;
const int       NUM_LIGHTS = 64;

const int       LIGHT_NUM_ENABLED_ROW = 3;

const long      LIGHT_PULSE_TIME = 1000;
const long      LIGHT_CYCLE_TIME = 150000;         // 8/3 * this yields blink time


/********************************************************************************/
/*                                                                              */
/*      entry points                                                            */      
/*                                                                              */
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
