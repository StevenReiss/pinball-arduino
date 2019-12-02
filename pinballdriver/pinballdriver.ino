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
static volatile int	watchdog_counter;
static boolean		default_testing = false;


/********************************************************************************/
/*										*/
/*	Forward definitions							*/
/*										*/
/********************************************************************************/

static void checkControlSwitches(void);
static void setupWatchdog(void);
static void watchdogReset(void);




/********************************************************************************/
/*										*/
/*	Setup code								*/
/*										*/
/********************************************************************************/

void setup()
{
   Serial.begin(115200);
   last_count = micros();

   pinMode(TEST_PIN, INPUT_PULLUP);
   pinMode(SOFT_RESET_PIN, INPUT_PULLUP);

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
   is_testing = default_testing;

   watchdog_counter = 0;

   next_test_switch_check = 0;
}




/********************************************************************************/
/*										*/
/*	Main processing loop							*/
/*										*/
/********************************************************************************/

void loop()
{
   watchdogReset();

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
      next_test_switch_check = addTime(now, TEST_SWITCH_CHECK_TIME);
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
   Serial.println("SOFT RESET");
   reset();
   delay(100);
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

      if (sts == LOW) {
	 ++test_down;
       }
      else if (test_down >= TEST_DOWN_CYCLES) {
	 Serial.println("START TESTING");
	 test_down = 0;
	 is_testing = true;
	 reset();
       }
      else if (test_down != 0) {
	 test_down = 0;
       }
      else {
	if (Serial.available() > 0) {
	   int input = Serial.read();
	   Serial.print("SERIAL IN ");
	   Serial.println(input);
	   switch (input) {
	      case 't' :
	      case 'T' :
		is_testing = true;
		reset();
		break;
	   }
	}
      }

    }

   int sts = digitalRead(SOFT_RESET_PIN);
   if (sts == LOW) {
      ++reset_down;
    }
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

unsigned long addTime(unsigned long t0, unsigned long t1)
{
   long t2 = t0 + t1;
   if (t2 < t0 || t2 < t1) t2 = MAX_TIME;
   return t2;
}


void writeBit(int pin, int word, int bit)
{
   int v = LOW;
   if (bitRead(word, bit)) v = HIGH;
   digitalWrite(pin, v);
}



/********************************************************************************/
/*										*/
/*	Watchdog methods							*/
/*										*/
/********************************************************************************/

static void setupWatchdog()
{
   noInterrupts();

   TCCR3A = 0;
   TCCR3B = 0;
   TCNT3 = 0;

   bitSet(TCCR3B, WGM12);
   bitSet(TCCR3B, CS21);

   bitSet(TCCR3B, WGM32);		 // ctc mode
   bitSet(TCCR3B, CS32);		 // 1024 prescaler
   bitSet(TCCR3B, CS30);

   OCR3A = 235;   // F_CPU / (1024 * 66) - 1;	   // should be every 15 ms

   bitSet(TIMSK3, OCIE3A);		 // enable interrupts

   watchdog_counter = 2;

   interrupts();
}


static void watchdogReset()
{
   ++watchdog_counter;
}


ISR(TIMER3_COMPA_vect)
{
   if (watchdog_counter == 0) {
      reset();
      Serial.println("WATCHDOG RESET");
    }

   if (watchdog_counter > 0) --watchdog_counter;
}




extern bool system_rtc_mem_read(uint8 src_addr, void *des_addr, uint16 load_size);
extern void __real_system_restart_local();

void __wrap_system_restart_local()
{
   static bool doing_reset = false;

   if (doing_reset) return;
   doing_reset = true;
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

   Serial.print("HARD RESET ");
   Serial.print(rst_info.reason);
   Serial.print(" ");
   Serial.print(rst_info.exccause);
   Serial.println();

   delayMicroseconds(10000);

   __real_system_restart_local();

}



/* end of pinballdriver.ino */
