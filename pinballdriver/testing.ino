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
static unsigned long next_light_change;
static int switch_down;
static int mem_protect;
static int auto_manual;
static int advance_btn;

static bool test_state;
static int display_counter;
static bool first_time;

struct DeltaTimer loop_timer;



/********************************************************************************/
/*										*/
/*	Forward definitions							*/
/*										*/
/********************************************************************************/

static void testingStart(void);
static void checkTestSwitches(void);
static void checkSerial(void);
static void nextTestMode(void);
static void updateTestMode(void);
static void setTestIdle(void);
static void endCurrentTest(void);
static void testSwitchOff(int);
static void testSwitchOn(int);
static void startSolenoidTest(void);
static void endSolenoidTest(void);
static void nextSolenoidTest(void);
static void startSoundTest(void);
static void endSoundTest(void);
static void nextSoundTest(void);
static void startLightTest(void);
static void endLightTest(void);
static void nextLightTest(void);
static void randomizeLights(void);
static void displayCount(void);
static void startSpecialTest(void);
static void endSpecialTest(void);
static void nextSpecialTest(void);
static void deltaTimerReport(DeltaTimerP);




/********************************************************************************/
/*										*/
/*	Setup methods for testing						*/
/*										*/
/********************************************************************************/

void testingSetup() {
  test_mode = TEST_IDLE;
  last_test_mode = TEST_IDLE;
  next_test_check = 0;
  next_test_update = 0;
  next_display_count = 0;
  next_light_change = 0;
  switch_down = 0;
  test_counter = 0;
  display_counter = 11111;
  first_time = true;
  mem_protect = HIGH;
  auto_manual = HIGH;
  advance_btn = HIGH;

  pinMode(TEST_PIN_MEM_PROTECT, INPUT_PULLUP);
  pinMode(TEST_PIN_AUTO_MANUAL, INPUT_PULLUP);
  // pinMode(TEST_PIN_BUTTON,INPUT_PULLUP);	// done in main driver
}



static void testingStart() {
  first_time = false;

  // make sure everything is off
  reset();

  Serial.println("TESTING START");
  default_testing = true;

  test_mode = TEST_IDLE;
  next_test_check = 0;
  next_test_update = 0;
  next_display_count = 0;
  switch_down = 0;
  test_counter = 0;
  display_counter = 11111;
  for (int i = 0; i < NUM_SCORE_DISPLAY; ++i) blankDisplay(i);
  blankDisplayLeft();
  blankDisplayRight();
  disableAllLights();
}



/********************************************************************************/
/*										*/
/*	Time wrap methods for testing						*/
/*										*/
/********************************************************************************/

void testingWrap() {
  Serial.println("TESTING WRAP");

  if (next_test_check != 0) next_test_check = 1;
  next_test_update = 0;
  next_display_count = 1;
  next_light_change = 0;
}




/********************************************************************************/
/*										*/
/*	Main loop methods for testing						*/
/*										*/
/********************************************************************************/

void testingUpdate(unsigned long now) {
  if (first_time) {
    testingStart();
    setupDeltaTimer(&loop_timer, "LOOP", 10);
  }

  deltaTimer(&loop_timer);

  if (now >= next_test_update) {
    handleSwitchChanges(4, testSwitchOff, testSwitchOn);
    next_test_update = addTime(now, TEST_CHECK_TIME);
    checkTestSwitches();
  }

  if (next_test_check > 0 && now >= next_test_check) {
    updateTestMode();
  }

  if (now >= next_display_count) {
    displayCount();
    next_display_count = addTime(now, TEST_DISPLAY_COUNT_INTERVAL);
  }

  if (now > next_light_change) {
    randomizeLights();
    next_light_change = addTime(now, TEST_LIGHT_RANDOMIZE);
  }

  checkSerial();
}



void testingReset() {
  Serial.println("TESTING RESET");
}



/********************************************************************************/
/*										*/
/*	Check test switch							*/
/*										*/
/********************************************************************************/

static void checkTestSwitches() {
  int sts = digitalRead(TEST_PIN);
  if (sts == LOW) {
    switch_down++;
  } else if (switch_down <= TEST_BOUNCE) {
    switch_down = 0;
  } else if (switch_down > 0) {
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
}


static void checkSerial() {
  if (Serial.available() == 0) return;

  int input = Serial.read();
  Serial.print("SERIAL IN ");
  Serial.println(input);
  switch (input) {
    case ' ':
      nextTestMode();
      break;
    case 'i':
    case 'I':
      endCurrentTest();
      setTestIdle();
      break;
    case 's':
    case 'S':
      endCurrentTest();
      startSolenoidTest();
      break;
    case 'A':
    case 'a':
    case 'N':
    case 'n':
      endCurrentTest();
      startSoundTest();
      break;
    case 'l':
    case 'L':
      endCurrentTest();
      startLightTest();
      break;
    case 'p':
    case 'P':
      endCurrentTest();
      startSpecialTest();
      break;
    case 'R':
      default_testing = false;
      reset();
      setup();
      break;
    default:
      break;
  }
}



static void nextTestMode() {
  if (next_display_count == 0)
    next_display_count = addTime(micros(), TEST_DISPLAY_COUNT_INTERVAL);

  switch (test_mode) {
    case TEST_IDLE:
      break;
    case TEST_SOLENOIDS:
      ++test_counter;
      if (test_counter < NUM_SOLENOID) return;
      break;
    case TEST_SOUNDS:
      ++test_counter;
      if (test_counter < NUM_SOUND) return;
      break;
    case TEST_LIGHTS:
      ++test_counter;
      if (test_counter < NUM_LIGHTS) {
        //		      next_test_check = 0;
        return;
      }
      disableAllLights();
      break;
    case TEST_SPECIALS:
      ++test_counter;
      if (test_counter < NUM_SPECIAL + 1) return;
      break;
  }

  endCurrentTest();

  next_test_check = 0;
  switch (last_test_mode) {
    default:
    case TEST_IDLE:
      startSolenoidTest();
      break;
    case TEST_SOLENOIDS:
      startSoundTest();
      break;
    case TEST_SOUNDS:
      startLightTest();
      break;
    case TEST_LIGHTS:
      startSpecialTest();
      break;
    case TEST_SPECIALS:
      setTestIdle();
      break;
  }
  last_test_mode = test_mode;
}


static void updateTestMode() {
  last_test_mode = test_mode;
  switch (test_mode) {
    default:
    case TEST_IDLE:
      break;
    case TEST_SOLENOIDS:
      nextSolenoidTest();
      break;
    case TEST_SOUNDS:
      nextSoundTest();
      break;
    case TEST_LIGHTS:
      nextLightTest();
      break;
    case TEST_SPECIALS:
      nextSpecialTest();
      break;
  }
}



static void setTestIdle() {
  test_mode = TEST_IDLE;
  next_test_check = 0;
}



static void endCurrentTest() {
  last_test_mode = test_mode;

  switch (test_mode) {
    default:
    case TEST_IDLE:
      break;
    case TEST_SOLENOIDS:
      endSolenoidTest();
      break;
    case TEST_SOUNDS:
      endSoundTest();
      break;
    case TEST_LIGHTS:
      endLightTest();
      break;
    case TEST_SPECIALS:
      endSpecialTest();
      break;
  }
}


/********************************************************************************/
/*										*/
/*	Switch testing								*/
/*										*/
/********************************************************************************/

static void testSwitchOff(int sw) {
  Serial.print("SWITCH OFF ");
  Serial.println(sw);
}



static void testSwitchOn(int sw) {
  Serial.print("SWITCH ON ");
  Serial.println(sw);
}



/********************************************************************************/
/*										*/
/*	Solenoid testing							*/
/*										*/
/********************************************************************************/

static void startSolenoidTest() {
  test_counter = 0;
  test_mode = TEST_SOLENOIDS;
  test_state = false;
  next_test_check = addTime(micros(), TEST_START_INTERVAL);
}

static void endSolenoidTest() {}


static void nextSolenoidTest() {
  if (test_counter > NUM_SOLENOID) {
    setTestIdle();
  } else {
    Serial.print("TRIGGER SOLENOID ");
    Serial.println(test_counter);
    queueSolenoid(test_counter);
    next_test_check = addTime(micros(), TEST_SOLENOID_INTERVAL);
  }
}




/********************************************************************************/
/*										*/
/*	Sound testing								*/
/*										*/
/********************************************************************************/

static void startSoundTest() {
  test_counter = 0;
  test_mode = TEST_SOUNDS;
  test_state = false;
  next_test_check = addTime(micros(), TEST_START_INTERVAL);
}

static void endSoundTest() {
}

static void nextSoundTest() {
  if (test_counter >= NUM_SOUND) {
    setTestIdle();
  } else {
    Serial.print("TRIGGER SOUND ");
    Serial.println(test_counter);
    queueSound(test_counter);
    next_test_check = addTime(micros(), TEST_SOUND_INTERVAL);
  }
}



/********************************************************************************/
/*										*/
/*	Light testing								*/
/*										*/
/********************************************************************************/

static void startLightTest() {
  test_mode = TEST_LIGHTS;
  test_counter = 0;
  test_state = false;
  next_test_check = addTime(micros(), TEST_START_INTERVAL);
  disableAllLights();
}


static void endLightTest() {
  disableAllLights();
}


static void nextLightTest() {
  if (test_counter > NUM_LIGHTS) {
    setTestIdle();
  } else {
    Serial.print("TURN ON LIGHT ");
    Serial.println(test_counter);
    disableAllLights();
    lightOn(test_counter);
    next_test_check = addTime(micros(), TEST_LIGHT_INTERVAL);
  }
}


static void randomizeLights() {
  if (test_mode == TEST_LIGHTS) return;

  disableAllLights();

  for (int i = 0; i < NUM_LIGHTS; ++i) {
    if (random(4) == 1) lightOn(i);
  }
}



/********************************************************************************/
/*										*/
/*	Display testing 							*/
/*										*/
/********************************************************************************/

static void displayCount() {
  for (int i = 0; i < NUM_SCORE_DISPLAY; ++i) {
    int v = display_counter - i * 111;
    if (v < 0) v = 0;
    setDisplay(i, v);
    // Serial.print("DISPLAY ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.println(v);
  }

  int vl = (display_counter) % 100;
  int vr = (vl + 11) % 100;

  setDisplayLeft(vl);
  setDisplayRight(vr);

  ++display_counter;
}




/********************************************************************************/
/*										*/
/*	Special testing 							*/
/*										*/
/********************************************************************************/

static void startSpecialTest() {
  test_mode = TEST_SPECIALS;
  test_counter = 0;
  test_state = false;
  next_test_check = addTime(micros(), TEST_START_INTERVAL);
  specialEnable();
  Serial.println("SPECIAL ENABLE");
}


static void endSpecialTest() {
  specialDisable();
  Serial.println("SPECIAL DISABLE");
}



static void nextSpecialTest() {
  if (test_counter > NUM_SPECIAL + 1) {
    setTestIdle();
  } else if (test_counter == 0)
    ;
  else {
    Serial.print("TRIGGER SPECIAL ");
    Serial.println(test_counter - 1);
    triggerSpecial(test_counter - 1);
    next_test_check = addTime(micros(), TEST_SPECIAL_INTERVAL);
  }
}


/********************************************************************************/
/*										*/
/*	Delta time management							*/
/*										*/
/********************************************************************************/

void setupDeltaTimer(DeltaTimerP timer, const char* name, int numrpt) {
  timer->time_last = 0;
  timer->time_count = 0;
  timer->time_total = 0;
  timer->time_max = 0;
  timer->time_reports = numrpt;
  timer->time_name = name;
}




void deltaTimer(DeltaTimerP timer) {
  if (!is_testing) return;
  if (timer->time_reports == 0) return;

  unsigned long now = micros();
  if (now < timer->time_last) {
    timer->time_last = 0;
  }
  if (timer->time_last != 0) {
    long delta = now - timer->time_last;
    if (delta > timer->time_max) timer->time_max = delta;
    timer->time_total += delta;
    ++timer->time_count;
    if ((timer->time_count % 10240) == 0) deltaTimerReport(timer);
  }
  timer->time_last = now;
}



static void deltaTimerReport(DeltaTimerP timer) {
  if (timer->time_count == 0) return;

  long avg = timer->time_total / timer->time_count;

  Serial.print(timer->time_name);
  Serial.print(" TIME: ");
  Serial.print(avg);
  Serial.print(" ");
  Serial.print(timer->time_max);
  Serial.print(" ");
  Serial.print(timer->time_count);
  Serial.println();

  if (timer->time_reports > 0) {
    timer->time_reports--;
  }
}



/* end of testing.ino */
