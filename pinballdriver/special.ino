/********************************************************************************/
/*										*/
/*	special.ino -- manage direct solenoids and flippers			*/
/*										*/
/********************************************************************************/




/********************************************************************************/
/*										*/
/*	 Private storage							*/
/*										*/
/********************************************************************************/

static unsigned long next_special_check;
static unsigned long next_special_off;

static int special_switch;    // which switch is on
static bool special_disable;  // disable flag
static int test_switch;





/********************************************************************************/
/*										*/
/*	Special access methods							*/
/*										*/
/********************************************************************************/

void specialDisable() {
  special_disable = true;
  digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE, LOW);
  removeSpecialSolenoid();
}


void specialEnable() {
  special_disable = false;
  digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE, HIGH);
}


void triggerSpecial(int id) {
  test_switch = id;
}



/********************************************************************************/
/*										*/
/*	Special setup methods							*/
/*										*/
/********************************************************************************/

void specialSetup() {
  pinMode(SPECIAL_PIN_OUT_SELECT0, OUTPUT);
  pinMode(SPECIAL_PIN_OUT_SELECT1, OUTPUT);
  pinMode(SPECIAL_PIN_OUT_SELECT2, OUTPUT);
  pinMode(SPECIAL_PIN_IN_0, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_IN_1, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_IN_2, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_IN_3, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_IN_4, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_IN_5, INPUT_PULLUP);
  pinMode(SPECIAL_PIN_DRIVER, OUTPUT);
  pinMode(SPECIAL_PIN_FLIPPER_ENABLE, OUTPUT);

  next_special_check = 0;
  next_special_off = 0;
  special_disable = true;
  special_switch = NO_SPECIAL;
  test_switch = -1;

  digitalWrite(SPECIAL_PIN_OUT_SELECT0, LOW);
  digitalWrite(SPECIAL_PIN_OUT_SELECT1, LOW);
  digitalWrite(SPECIAL_PIN_OUT_SELECT2, LOW);
  digitalWrite(SPECIAL_PIN_DRIVER, LOW);
  digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE, LOW);

  specialDisable();
}




/********************************************************************************/
/*										*/
/*	Special time wrap methods						*/
/*										*/
/********************************************************************************/

void specialWrap() {
  next_special_check = 0;
  if (next_special_off > 0) next_special_off = 1;
  else next_special_off = 0;
}




/********************************************************************************/
/*										*/
/*	Special update methods							*/
/*										*/
/********************************************************************************/

void specialUpdate(unsigned long now) {
  if (next_special_off > 0 && now >= next_special_off) {
    test_switch = -1;
    removeSpecialSolenoid();
  }
  if (now >= next_special_check && next_special_off > 0) {
    checkOffSwitch();
  }
  if (now >= next_special_check && next_special_off == 0) {
    checkSwitch(now);
    next_special_check = addTime(now, SPECIAL_CHECK_TIME);
  }
}



/********************************************************************************/
/*										*/
/*	Special reset methods							*/
/*										*/
/********************************************************************************/

void specialReset() {
  specialDisable();
}



/********************************************************************************/
/*										*/
/*	Special action methods							*/
/*										*/
/********************************************************************************/


static void checkSwitch(unsigned long now) {
  for (int i = 0; i < NUM_SPECIAL; ++i) {
    int spec = (i + 2) % NUM_SPECIAL;
    int sts = digitalRead(SPECIAL_PIN_IN(spec));
    if (spec == test_switch) {
      sts = SPECIAL_ON;
    }
    if (sts == SPECIAL_ON) {
      if (special_switch != spec) {
        if (is_testing) {
          Serial.print("DETECT SPECIAL ");
          Serial.println(spec);
        }
        special_switch = spec;
        if (!special_disable) turnOnSpecialSolenoid(spec, now);
        else special_switch = NO_SPECIAL;
      }
      break;
    }
  }
}



static void checkOffSwitch() {
  if (special_switch == NO_SPECIAL) return;
  int sts = digitalRead(SPECIAL_PIN_IN(special_switch));
  if (test_switch == special_switch) sts = SPECIAL_ON;
  if (sts != SPECIAL_ON) removeSpecialSolenoid();
}




static void removeSpecialSolenoid() {
  if (!special_disable && is_testing) Serial.println("SPECIAL SOLENOID OFF");
  digitalWrite(SPECIAL_PIN_DRIVER, LOW);
  next_special_off = 0;
  special_switch = NO_SPECIAL;
  test_switch = -1;
}



static void turnOnSpecialSolenoid(int spec, unsigned long now) {
  if (spec == NO_SPECIAL || special_disable) {
    removeSpecialSolenoid();
  } else {
    int spno = SPECIAL_OUT(spec);
    digitalWrite(SPECIAL_PIN_DRIVER, LOW);
    if (is_testing) {
      Serial.print("SPECIAL SOLENOID ON ");
      Serial.print(spec);
      Serial.print(" ");
      Serial.println(spno);
    }
    writeBit(SPECIAL_PIN_OUT_SELECT0, spno, 0);
    writeBit(SPECIAL_PIN_OUT_SELECT1, spno, 1);
    writeBit(SPECIAL_PIN_OUT_SELECT2, spno, 2);
    digitalWrite(SPECIAL_PIN_DRIVER, HIGH);
    next_special_off = addTime(now, SPECIAL_ON_TIME);
  }
}




/* end of special.ino */
