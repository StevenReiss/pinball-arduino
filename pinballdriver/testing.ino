/********************************************************************************/
/*										*/
/*	testing.ino -- test program for the pinball machine			*/
/*										*/
/********************************************************************************/

// need to add light testing -- turn on all lights?, random lights as logic
//    light patterns

// use Serial.available and Serial.readString or parseInt to simulate test
//    switch


/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static TestMode test_mode;
static unsigned long next_test_update;
static unsigned long next_test_check;
static unsigned long next_display_count;
static int switch_down;

static int test_counter;
static bool test_state;
static int display_counter;
static bool first_time;



/********************************************************************************/
/*										*/
/*	Setup methods for testing						*/
/*										*/
/********************************************************************************/

void testingSetup()
{
   test_mode = TEST_IDLE;
   next_test_check = 0;
   next_test_update = 0;
   next_display_count = 0;
   switch_down = 0;
   test_counter = 0;
   display_counter = 0; 
   first_time = true;
}



void testingStart()
{
   first_time = false;

   Serial.println("TESTING START");

   test_mode = TEST_IDLE;
   next_test_check = 0;
   next_test_update = 0;
   next_display_count = 0;
   switch_down = 0;
   test_counter = 0;
   display_counter = 0;
   for (int i = 0; i < NUM_SCORE_DISPLAY; ++i) blankDisplay(i);
   blankDisplayLeft();
   blankDisplayRight();
}



/********************************************************************************/
/*										*/
/*	Time wrap methods for testing						*/
/*										*/
/********************************************************************************/

void testingWrap()
{
   Serial.println("TESTING WRAP");

   if (next_test_check != 0) next_test_check = 1;
   next_test_update = 0;
   next_display_count = 1;
}




/********************************************************************************/
/*										*/
/*	Main loop methods for testing						*/
/*										*/
/********************************************************************************/

void testingUpdate(unsigned long now)
{
   if (first_time) testingStart();

   if (now >= next_test_update) {
      checkTestSwitch();
      handleSwitchChanges(4,testSwitchOff,testSwitchOn);
      next_test_update = addTime(now,TEST_CHECK_TIME);
    }

   if (next_test_check > 0 && now >= next_test_check) {
      updateTestMode();
    }

   if (now >= next_display_count) {
      displayCount();
      next_display_count = addTime(now,TEST_DISPLAY_COUNT_INTERVAL);
    }
}



void testingReset()
{
   Serial.println("TESTING RESET");
}



/********************************************************************************/
/*										*/
/*	Check test switch							*/
/*										*/
/********************************************************************************/

static void checkTestSwitch()
{
   int sts = digitalRead(TEST_PIN);
   if (sts == LOW) {
      switch_down++;
    }
   else if (switch_down <= TEST_BOUNCE) {
      switch_down = 0;
    }
   else {
      nextTestMode();
    }
}



static void nextTestMode()
{
   next_test_check = 0;
   if (next_display_count == 0) next_display_count = addTime(micros(),TEST_DISPLAY_COUNT_INTERVAL);

   switch (test_mode) {
      default :
      case TEST_IDLE :
	 startSolenoidTest();
	 break;
      case TEST_SOLENOIDS :
	 startSoundTest();
	 break;
      case TEST_SOUNDS :
	 startLightTest();
	 break;
      case TEST_LIGHTS :
	 setTestIdle();
	 break;
    }
}


static void updateTestMode()
{
   switch(test_mode) {
      default :
      case TEST_IDLE :
	 break;
      case TEST_SOLENOIDS :
	 nextSolenoidTest();
	 break;
      case TEST_SOUNDS :
	 nextSoundTest();
	 break;
      case TEST_LIGHTS :
	 nextLightTest();
	 break;
    }
}


static void setTestIdle()
{
   test_mode = TEST_IDLE;
   next_test_check = 0;
}




/********************************************************************************/
/*										*/
/*	Switch testing								*/
/*										*/
/********************************************************************************/

static void testSwitchOff(int sw)
{
   Serial.print("SWITCH OFF ");
   Serial.println(sw);
}



static void testSwitchOn(int sw)
{
   Serial.print("SWITCH ON ");
   Serial.println(sw);
}



/********************************************************************************/
/*										*/
/*	Solenoid testing							*/
/*										*/
/********************************************************************************/

static void startSolenoidTest()
{
   test_counter = 0;
   test_mode = TEST_SOLENOIDS;
   test_state = false;
   next_test_check = addTime(micros(),TEST_START_INTERVAL);
}



static void nextSolenoidTest()
{
   if (test_counter > NUM_SOLENOID) {
      setTestIdle();
    }
   else if (test_counter == NUM_SOLENOID) {
      Serial.println("TRIGGER ALL SOLENOIDS");
      for (int i = 0; i < NUM_SOLENOID; ++i) queueSolenoid(i);
      test_counter++;
      next_test_check = addTime(micros(),TEST_SOLENOID_INTERVAL);
    }
   else {
      Serial.print("TRIGGER SOLENOID ");
      Serial.println(test_counter);
      queueSolenoid(test_counter);
      test_counter++;
      next_test_check = addTime(micros(),TEST_SOLENOID_INTERVAL);
    }
}




/********************************************************************************/
/*										*/
/*	Sound testing								*/
/*										*/
/********************************************************************************/

static void startSoundTest()
{
   test_counter = 0;
   test_mode = TEST_SOUNDS;
   test_state = false;
   next_test_check = addTime(micros(),TEST_START_INTERVAL);
}



static void nextSoundTest()
{
   if (test_counter >= NUM_SOUND) {
      setTestIdle();
    }
   else {
      Serial.print("TRIGGER SOUND ");
      Serial.println(test_counter);
      queueSound(test_counter);
      test_counter++;
      next_test_check = addTime(micros(),TEST_SOUND_INTERVAL);
    }
}



/********************************************************************************/
/*										*/
/*	Light testing								*/
/*										*/
/********************************************************************************/

static void startLightTest()
{
   test_mode = TEST_LIGHTS;
   test_counter = 0;
   test_state = false;
   next_test_check = addTime(micros(),TEST_START_INTERVAL);
   disableAllLights();
}



static void nextLightTest()
{
   if (test_counter > NUM_LIGHTS) {
      setTestIdle();
    }
   else {
      Serial.print("TURN ON LIGHT ");
      Serial.println(test_counter);
      if (test_counter > 0) lightOff(test_counter-1);
      lightOn(test_counter);
      test_counter++;
      next_test_check = addTime(micros(),TEST_LIGHT_INTERVAL);
    }
}




/********************************************************************************/
/*										*/
/*	Display testing 							*/
/*										*/
/********************************************************************************/

static void displayCount()
{
   for (int i = 0; i < NUM_SCORE_DISPLAY; ++i) {
      int v = display_counter - i*111;
      if (v < 0) v = 0;
      setDisplay(i,v);
    }

   int vl = (display_counter/1000) % 100;
   int vr = (vl + 11) % 100;

   setDisplayLeft(vl);
   setDisplayRight(vr);

   ++display_counter;

}



/* end of testing.ino */
