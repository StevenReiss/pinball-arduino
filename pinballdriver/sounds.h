/********************************************************************************/
/*										*/
/*	sounds.h -- low level code for sounds					*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	Pin Assignments 							*/
/*										*/
/********************************************************************************/

const int SOUND_PIN_SELECT0 = SOUND_PIN_START + 0;
const int SOUND_PIN_SELECT1 = SOUND_PIN_START + 2;
const int SOUND_PIN_SELECT2 = SOUND_PIN_START + 4;
const int SOUND_PIN_DRIVER = SOUND_PIN_START + 6;

const int SOUND_PLAY_PWM_PIN = 12;
const int SOUND_PLAY_ALT_PWM_PIN = 11;

// const int SOUND_PIN_ALTERNATE_SET = SOUND_PIN_START + 8;
const int SOUND_PIN_ALTERNATE_SET = SOUND_PLAY_PWM_PIN; 	// We didn't wire the ALTERNATE PIN, use pwm for now



/********************************************************************************/
/*										*/
/*	Sound Constants 							*/
/*										*/
/********************************************************************************/

const int NUM_SOUND = 9;

const int SOUND_QUEUE_SIZE = NUM_SOUND*4;

const long SOUND_ON_TIME = 100;
const long SOUND_OFF_TIME = 0;
const long SOUND_CHECK_TIME = 5000;

const int SOUND_OFF = LOW;
const int SOUND_ON = HIGH;

const int SOUND_PLAY_TIMER = 1; 	// pins 11 and 12




/********************************************************************************/
/*										*/
/*	Entry points								*/
/*										*/
/********************************************************************************/

extern void queueSound(int);

extern void soundsSetup();
extern void soundsWrap();
extern void soundsUpdate(unsigned long);
extern void soundsReset(void);



/* end of sounds.h */
