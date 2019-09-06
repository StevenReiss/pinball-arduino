/********************************************************************************/
/*										*/
/*	testing.ino -- test program for the pinball machine			*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static TestMode test_mode;
static TestMode last_test_mode;
static int test_counter;

static unsigned long next_test_update;
static unsigned long next_test_check;
static unsigned long next_display_count;
static int switch_down;
static int mem_protect;
static int auto_manual;
static int advance_btn;

static long testing_total_time;
static long testing_num_times;
static long testing_last_time;
static long testing_num_total;


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
   last_test_mode = TEST_IDLE;
   next_test_check = 0;
   next_test_update = 0;
   next_display_count = 0;
   switch_down = 0;
   test_counter = 0;
   display_counter = 0;
   first_time = true;
   mem_protect = HIGH;
   auto_manual = HIGH;
   advance_btn = HIGH;

   testing_last_time = 0;
   testing_total_time = 0;
   testing_num_times = 0;
   testing_num_total = 0;

   pinMode(TEST_PIN_MEM_PROTECT,INPUT_PULLUP);
   pinMode(TEST_PIN_AUTO_MANUAL,INPUT_PULLUP);
   pinMode(TEST_PIN_ADVANCE,INPUT_PULLUP);
   // pinMode(TEST_PIN_BUTTON,INPUT_PULLUP);	// done in main driver
}



static void testingStart()
{
   first_time = false;

   // make sure everything is off
   reset();

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
   if (first_time) {
      testingStart();
      testing_last_time = now;
    }
   else if (testing_num_times < 10000) {
      ++testing_num_times;
      if (testing_num_times % 1000 == 0) {
	 testing_total_time += now - testing_last_time;
	 ++testing_num_total;
	 Serial.print("TESTING CYCLE TIME = ");
	 Serial.println(now - testing_last_time);
       }
      testing_last_time = now;
    }
   else if (testing_num_times == 10000) {
      ++testing_num_times;
      Serial.print("TESTING AVG CYCLE TIME = ");
      Serial.println(testing_total_time / testing_num_total);
    }

   if (now >= next_test_update) {
      handleSwitchChanges(4,testSwitchOff,testSwitchOn);
      next_test_update = addTime(now,TEST_CHECK_TIME);
      checkTestSwitches();
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

static void checkTestSwitches()
{
   int sts = digitalRead(TEST_PIN);
   if (sts == LOW) {
      switch_down++;
    }
   else if (switch_down <= TEST_BOUNCE) {
      switch_down = 0;
    }
   else if (switch_down > 0) {
      nextTestMode();
      Serial.print("TESTING NEXT MODE ");
      Serial.print(test_mode);
      Serial.print(" ");
      Serial.println(test_counter);

      switch_down = 0;
    }

   int sts1 = digitalRead(TEST_PIN_MEM_PROTECT);
   if (sts1 != mem_protect) {
      mem_protect = sts1;
      Serial.print("TEST MEM_PROTECT ");
      Serial.println(sts1);
    }

   int sts2 = digitalRead(TEST_PIN_AUTO_MANUAL);
   if (sts2 != auto_manual) {
      auto_manual = sts2;
      Serial.print("TEST AUTO_MANUAL ");
      Serial.println(sts2);
    }

   int sts3 = digitalRead(TEST_PIN_ADVANCE);
   if (sts3 != advance_btn) {
      advance_btn = sts3;
      Serial.print("TEST ADVANCE ");
      Serial.println(sts3);
    }
}



static void nextTestMode()
{
   if (next_display_count == 0)
      next_display_count = addTime(micros(),TEST_DISPLAY_COUNT_INTERVAL);

   switch (test_mode) {
      case TEST_IDLE :
	 break;
      case TEST_SOLENOIDS :
	 ++test_counter;
	 if (test_counter < NUM_SOLENOID) return;
	 break;
      case TEST_SOUNDS :
	 ++test_counter;
	 if (test_counter < NUM_SOUND) return;
	 break;
      case TEST_LIGHTS :
	 ++test_counter;
	 if (test_counter < NUM_LIGHTS) return;
	 break;
      case TEST_SPECIALS :
	 ++test_counter;
	 if (test_counter < NUM_SPECIAL) return;
	 break;
    }

   next_test_check = 0;
   switch (last_test_mode) {
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
	 startSpecialTest();
	 break;
      case TEST_SPECIALS :
	 setTestIdle();
	 break;
    }
   last_test_mode = test_mode;
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
      case TEST_SPECIALS :
	 nextSpecialTest();
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
   else {
      Serial.print("TRIGGER SOLENOID ");
      Serial.println(test_counter);
      queueSolenoid(test_counter);
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

   int vl = (display_counter) % 100;
   int vr = (vl + 11) % 100;

   setDisplayLeft(vl);
   setDisplayRight(vr);
   //	  setDisplayLeft(67);
   //	  setDisplayRight(89);

   ++display_counter;
}




/********************************************************************************/
/*										*/
/*	Special testing 							*/
/*										*/
/********************************************************************************/

static void startSpecialTest()
{
   test_mode = TEST_SPECIALS;
   test_counter = 0;
   test_state = false;
   next_test_check = addTime(micros(),TEST_START_INTERVAL);
}



static void nextSpecialTest()
{
   if (test_counter > NUM_SPECIAL) {
      setTestIdle();
    }
   else {
      Serial.print("TRIGGER SPECIAL ");
      Serial.println(test_counter);
      triggerSpecial(test_counter);
      next_test_check = addTime(micros(),TEST_SPECIAL_INTERVAL);
    }
   setTestIdle();
}



/* end of testing.ino */
