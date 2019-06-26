/********************************************************************************/
/*										*/
/*	pinballdriver.ino -- main program for arduino pinball			*/
/*										*/
/********************************************************************************/

#include "pinballdriver.h"
#include "display.h"
#include "lights.h"
#include "switches.h"
#include "solenoids.h"
#include "sounds.h"
#include "special.h"
#include "sound_data.h"

#include "logic.h"
#include "testing.h"

#include <EEPROM.h>
#include <stddef.h>



/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static unsigned long last_count;
static unsigned long next_test_switch_check;
static bool		is_testing;
static byte		test_down;
static byte		reset_down;




/********************************************************************************/
/*										*/
/*	Setup code								*/
/*										*/
/********************************************************************************/

void setup()
{
   Serial.begin(57600);
   last_count = micros();

   pinMode(TEST_PIN,INPUT_PULLUP);
   pinMode(SOFT_RESET_PIN,INPUT_PULLUP);

   displaySetup();
   lightsSetup();
   solenoidsSetup();
   soundsSetup();
   specialSetup();
   switchesSetup();

   logicSetup();
   testingSetup();

   test_down = 0;
   reset_down = 0;
   is_testing = false;

   next_test_switch_check = 0;
}




/********************************************************************************/
/*										*/
/*	Main processing loop							*/
/*										*/
/********************************************************************************/

void loop()
{
   unsigned long now = micros();
   if (now < last_count || now == MAX_TIME) {
      displayWrap();
      lightsWrap();
      solenoidsWrap();
      soundsWrap();
      specialWrap();
      switchesWrap();
      logicWrap();
      testingWrap();
      now = micros();
      next_test_switch_check = TEST_SWITCH_CHECK_TIME;
    }

   last_count = now;

   displayUpdate(now);
   lightsUpdate(now);
   solenoidsUpdate(now);
   soundsUpdate(now);
   specialUpdate(now);
   switchesUpdate(now);

   if (is_testing) testingUpdate(now);
   else logicUpdate(now);

   if (now > next_test_switch_check) {
      checkControlSwitches();
      next_test_switch_check = addTime(now,TEST_SWITCH_CHECK_TIME);
    }
}



/********************************************************************************/
/*										*/
/*	Soft reset methods							*/
/*										*/
/********************************************************************************/

void reset()
{
   displayReset();
   lightsReset();
   solenoidsReset();
   soundsReset();
   specialReset();
   switchesReset();
   testingReset();
   logicReset();
}


void (*forceResetFunc)(void) = 0;

void softReset()
{
   reset();
   forceResetFunc();
}



/********************************************************************************/
/*										*/
/*	Control switch checking 						*/
/*										*/
/********************************************************************************/

static void checkControlSwitches()
{
   if (!is_testing) {
      int sts = digitalRead(TEST_PIN);
      if (sts == LOW) ++test_down;
      else if (test_down >= TEST_DOWN_CYCLES) {
	 test_down = 0;
	 is_testing = true;
	 reset();
       }
      else test_down = 0;
    }
   int sts = digitalRead(SOFT_RESET_PIN);
   if (sts == LOW) ++reset_down;
   else if (reset_down >= RESET_DOWN_CYCLES) {
      reset_down = 0;
      softReset();
    }
   else reset_down = 0;
}



/********************************************************************************/
/*										*/
/*	Helper methods								*/
/*										*/
/********************************************************************************/

unsigned long addTime(unsigned long t0,unsigned long t1)
{
   long t2 = t0 + t1;
   if (t2 < t0 || t2 < t1) t2 = MAX_TIME;
   return t2;
}



/* end of pinballdriver.ino */


