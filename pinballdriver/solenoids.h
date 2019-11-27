/********************************************************************************/
/*										*/
/*	solenoids.h -- low level code for regular solenoids			*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	Pin Assignments 							*/
/*										*/
/********************************************************************************/

const int SOLENOID_PIN_SELECT0 = SOLENOID_PIN_START + 0;
const int SOLENOID_PIN_SELECT1 = SOLENOID_PIN_START + 2;
const int SOLENOID_PIN_SELECT2 = SOLENOID_PIN_START + 4;
const int SOLENOID_PIN_SELECT3 = SOLENOID_PIN_START + 6;
const int SOLENOID_PIN_DRIVER =  SOLENOID_PIN_START + 8;



/********************************************************************************/
/*										*/
/*	Solenoid Constants							*/
/*										*/
/********************************************************************************/

// note that solenoid 0 is not used (to avoid trigger on startup)
const int NUM_SOLENOID = 11;
const int FIRST_SOLENOID = 1;

const long SOLENOID_ON_TIME = 10000;		// Basic unit -- *# for solenoid
const long SOLENOID_OFF_TIME = 40000;
const long SOLENOID_CHECK_TIME = 5000;

const int SOLENOID_DRIVER_OFF = HIGH;
const int SOLENOID_DRIVER_ON = LOW;



/********************************************************************************/
/*										*/
/*	External entries							*/
/*										*/
/********************************************************************************/

extern void queueSolenoid(int);
extern void removeAllSolenoids();

extern void solenoidsSetup();
extern void solenoidsWrap();
extern void solenoidsUpdate(unsigned long);
extern void solenoidsReset(void);


/* end of solenoids.h */


