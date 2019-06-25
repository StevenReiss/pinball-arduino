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
static bool	     is_testing;
static unsigned long next_test_switch_check;



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

   is_testing = false;
   int sts = digitalRead(TEST_PIN);
   if (sts == LOW) is_testing = true;

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
      int sts = digitalRead(TEST_PIN);
      if (sts == LOW) is_testing = true;
      next_test_switch_check = addTime(now,TEST_SWITCH_CHECK_TIME);
      sts = digitalRead(SOFT_RESET_PIN);
      if (sts == LOW) softReset();
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
/*	Helper methods								*/
/*										*/
/********************************************************************************/

unsigned long addTime(unsigned long t0,unsigned long t1)
{
   long t2 = t0 + t1;
   if (t2 < t0 || t2 < t1) t2 = MAX_TIME;
   return t2;
}



/********************************************************************************/
/*										*/
/*	Soft resest handling							*/
/*										*/
/********************************************************************************/

extern bool system_rtc_mem_read(uint8 src_addr, void *des_addr, uint16 load_size);
extern void __real_system_restart_local();


void __wrap_system_restart_local()
{
   register uint32_t sp asm("a1");

   struct rst_info rst_info = {0};
   system_rtc_mem_read(0, &rst_info, sizeof(rst_info));
   if (rst_info.reason != REASON_SOFT_WDT_RST &&
	  rst_info.reason != REASON_EXCEPTION_RST &&
	  rst_info.reason != REASON_WDT_RST)
      {
      return;
    }

   reset();

   delayMicroseconds(10000);

   __real_system_restart_local();

}



/* end of pinballdriver.ino */
