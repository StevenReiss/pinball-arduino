/********************************************************************************/
/*                                                                              */
/*      special.h -- definitions for direct solenoids, flippers, etc.           */
/*                                                                              */
/********************************************************************************/




/********************************************************************************/
/*                                                                              */
/*      Pin Assignments                                                         */
/*                                                                              */
/********************************************************************************/

const int SPECIAL_PIN_IN_0 = SPECIAL_PIN_START;
const int SPECIAL_PIN_IN_1 = SPECIAL_PIN_START + 1;
const int SPECIAL_PIN_IN_2 = SPECIAL_PIN_START + 2;
const int SPECIAL_PIN_IN_3 = SPECIAL_PIN_START + 3;
const int SPECIAL_PIN_IN_4 = SPECIAL_PIN_START + 4;
const int SPECIAL_PIN_IN_5 = SPECIAL_PIN_START + 5;
const int SPECIAL_PIN_OUT_SELECT0 = SPECIAL_PIN_START + 6;
const int SPECIAL_PIN_OUT_SELECT1 = SPECIAL_PIN_START + 7;
const int SPECIAL_PIN_OUT_SELECT2 = SPECIAL_PIN_START + 8;

const int SPECIAL_PIN_DRIVER = SPECIAL_PIN_START2;
const int SPECIAL_PIN_FLIPPER_ENABLE = SPECIAL_PIN_START2 + 1;

#define SPECIAL_PIN_IN(i)       (SPECIAL_PIN_IN_0 + i)



/********************************************************************************/
/*                                                                              */
/*      Special constants                                                       */
/*                                                                              */
/********************************************************************************/

const long SPECIAL_CHECK_TIME = 1000;
const long SPECIAL_ON_TIME = 20000;
const long SPECIAL_OFF_TIME = 5000;

const int  NUM_SPECIAL = 6;

const int  NO_SPECIAL = 100;            // indicator of none on


const int SPECIAL_ON = LOW;



/********************************************************************************/
/*                                                                              */
/*      External definitions                                                    */
/*                                                                              */
/********************************************************************************/

extern void specialEnable();
extern void specialDisable();

extern void specialSetup();
extern void specialWrap();
extern void specialUpdate(unsigned long);



/* end of speical.h */

