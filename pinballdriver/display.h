/********************************************************************************/
/*										*/
/*	Constants for output displays						*/
/*										*/
/********************************************************************************/


/********************************************************************************/
/*										*/
/*	Pin assignments 							*/
/*										*/
/********************************************************************************/

const int DISPLAY_DATA_PIN = DISPLAY_PIN_START;
const int DISPLAY_DATA2_PIN = DISPLAY_PIN_START2;
const int DISPLAY_CLOCK_PIN = DISPLAY_PIN_START+1;
const int DISPLAY_CLOCK_MPX0_PIN = DISPLAY_PIN_START+2;
const int DISPLAY_CLOCK_MPX1_PIN = DISPLAY_PIN_START+3;
const int DISPLAY_CLOCK_MPX2_PIN = DISPLAY_PIN_START+4;
const int DISPLAY_LATCH_PIN = DISPLAY_PIN_START2+ 2;
const int DISPLAY_DIGIT1_PIN = DISPLAY_PIN_START2 + 4;
const int DISPLAY_DIGIT2_PIN = DISPLAY_PIN_START2 + 6;
const int DISPLAY_DIGIT3_PIN = DISPLAY_PIN_START2 + 8;

#define DISPLAY_DIGIT_PIN(i)	(DISPLAY_DIGIT1_PIN + i*2)



/********************************************************************************/
/*										*/
/*	Timing values								*/
/*										*/
/********************************************************************************/

const long DISPLAY_SET_INCR = 4000;	    // use to keep a digit on
					    //	 cycle among displays

const long DISPLAY_UPDATE_INCR = 100000;    // update digit tables every k ms



/********************************************************************************/
/*										*/
/*	Other relevant constants						*/
/*										*/
/********************************************************************************/

const long MAX_VALUE = 1000000;
const int MAX_HALF_VALUE = 1000;

const int DIGIT_SHOW = HIGH;
const int DIGIT_HIDE = LOW;

const int NUM_DISPLAY = 5;
const int NUM_SCORE_DISPLAY= 4;

const int DISPLAY_SPLIT = 4;




/********************************************************************************/
/*										*/
/*	Segment display data							*/
/*										*/
/********************************************************************************/

const unsigned int bits [] = {
   0xf5,	   // 0
   0x44,	   // 1
   0x73,	   // 2
   0x57,	   // 3
   0xc6,	   // 4
   0x97,	   // 5
   0xb7,	   // 6
   0x45,	   // 7
   0xf7,	   // 8
   0xc7,	   // 9
   0x00 	   // blank
};

const unsigned int DISPLAY_COMMA = 0x8;

const unsigned int DISPLAY_BLANK = 10;


/********************************************************************************/
/*										*/
/*	Routines								*/
/*										*/
/********************************************************************************/

extern void displaySetup();
extern void displayUpdate(unsigned long now);
extern void displayWrap();
extern void displayReset();

extern void blankDisplay(int i);
extern void setDisplay(int i,long v);
extern void setDisplayLeft(int i);
extern void setDisplayRight(int i);
extern void blankDisplayLeft(int i);
extern void blankDisplayRight(int i);




/* end of display.h */

