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



/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static unsigned long last_count;
static bool          is_testing;


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

   displaySetup();
   lightsSetup();
   solenoidsSetup();
   soundsSetup();
   specialSetup();
   switchesSetup();

   logicSetup();
   testingSetup();

   is_testing = false;
   int sts = digitalRead(TEST_PIN);
   if (sts == LOW) is_testing = true;
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
