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

static unsigned long	next_special_check;
static unsigned long	next_special_off;
static unsigned long	next_special_on;

static int		cur_special;		// what we are looking at
static int		special_switch; 	// which switch is on
static bool		special_disable;	// disable flag
static int              test_switch;





/********************************************************************************/
/*										*/
/*	Special access methods							*/
/*										*/
/********************************************************************************/

void specialDisable()
{
   special_disable = true;
   digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE,LOW);
   removeSpecialSolenoid();
}


void specialEnable()
{
   special_disable = false;
   digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE,HIGH);
}


void triggerSpecial(int id)
{
   test_switch = id;
}



/********************************************************************************/
/*										*/
/*	Special setup methods							*/
/*										*/
/********************************************************************************/

void specialSetup()
{
   pinMode(SPECIAL_PIN_OUT_SELECT0,OUTPUT);
   pinMode(SPECIAL_PIN_OUT_SELECT1,OUTPUT);
   pinMode(SPECIAL_PIN_OUT_SELECT2,OUTPUT);
   pinMode(SPECIAL_PIN_IN_0,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_IN_1,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_IN_2,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_IN_3,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_IN_4,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_IN_5,INPUT_PULLUP);
   pinMode(SPECIAL_PIN_DRIVER,OUTPUT);
   pinMode(SPECIAL_PIN_FLIPPER_ENABLE,OUTPUT);

   next_special_check = 0;
   next_special_off = 0;
   next_special_on = 0;
   special_disable = true;
   cur_special = 0;
   special_switch = NO_SPECIAL;
   test_switch = -1;

   digitalWrite(SPECIAL_PIN_OUT_SELECT0,LOW);
   digitalWrite(SPECIAL_PIN_OUT_SELECT1,LOW);
   digitalWrite(SPECIAL_PIN_OUT_SELECT2,LOW);
   digitalWrite(SPECIAL_PIN_DRIVER,LOW);
   digitalWrite(SPECIAL_PIN_FLIPPER_ENABLE,LOW);

   specialDisable();
}




/********************************************************************************/
/*										*/
/*	Special time wrap methods						*/
/*										*/
/********************************************************************************/

void specialWrap()
{
   next_special_check = 0;
   next_special_off = 0;
   next_special_on = 0;
}




/********************************************************************************/
/*										*/
/*	Special update methods							*/
/*										*/
/********************************************************************************/

void specialUpdate(unsigned long now)
{
   if (now >= next_special_check) {
      checkSwitch(now);
      next_special_check = addTime(now,SPECIAL_CHECK_TIME);
    }
   if (next_special_on > 0 && now >= next_special_on) {
      triggerSpecialSolenoid(now);
    }
   if (next_special_off > 0 && now >= next_special_off) {
      test_switch = -1;
      digitalWrite(SPECIAL_PIN_DRIVER,LOW);
      next_special_on = addTime(now,SPECIAL_OFF_TIME);
    }
}



/********************************************************************************/
/*										*/
/*	Special reset methods							*/
/*										*/
/********************************************************************************/

void specialReset()
{
   specialDisable();
}



/********************************************************************************/
/*										*/
/*	Special action methods							*/
/*										*/
/********************************************************************************/


static void checkSwitch(unsigned long now)
{
   if (special_disable) {
      special_switch = NO_SPECIAL;
      return;
    }

   if (special_switch != NO_SPECIAL) cur_special = special_switch;
   else cur_special = (cur_special + 1) % NUM_SPECIAL;

   for (int i = 0; i < NUM_SPECIAL; ++i) {
      int sts = digitalRead(SPECIAL_PIN_IN(cur_special));
      if (cur_special == test_switch) {
         sts = SPECIAL_ON;
       }
      if (sts == SPECIAL_ON) {
	 if (special_switch != cur_special) {
//            if (is_testing) {
               Serial.print("DETECT SPECIAL ");
               Serial.println(cur_special);
//            }
	    special_switch = cur_special;
	    triggerSpecialSolenoid(now);
	  }
	 break;
       }
      else if (special_switch != NO_SPECIAL) {
	 special_switch = NO_SPECIAL;
	 triggerSpecialSolenoid(now);
       }
      cur_special = (cur_special + 1) % NUM_SPECIAL;
    }
}



static void removeSpecialSolenoid()
{
   Serial.println("SPECIAL OFF");     
   digitalWrite(SPECIAL_PIN_DRIVER,LOW);
   next_special_off = 0;
   next_special_on = 0;
   special_switch = NO_SPECIAL;
}



static void triggerSpecialSolenoid(unsigned long now)
{
   if (special_switch == NO_SPECIAL) {
      removeSpecialSolenoid();
    }
   else {
      digitalWrite(SPECIAL_PIN_DRIVER,LOW);
      Serial.print("SPECIAL ON ");
      Serial.println(special_switch);
      writeBit(SPECIAL_PIN_OUT_SELECT0,special_switch,0);
      writeBit(SPECIAL_PIN_OUT_SELECT1,special_switch,1);
      writeBit(SPECIAL_PIN_OUT_SELECT2,special_switch,2);
      digitalWrite(SPECIAL_PIN_DRIVER,HIGH);
      next_special_off = addTime(now,SPECIAL_ON_TIME);
    }
}




/* end of special.ino */
