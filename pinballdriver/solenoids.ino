/********************************************************************************/
/*										*/
/*	solenoids.ino -- low level solenoid code				*/
/*										*/
/********************************************************************************/


/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static byte	solenoid_queue[NUM_SOLENOID];
static byte	solenoid_start;
static byte	solenoid_end;
static long	solenoid_queued;

static unsigned long next_solenoid;
static bool	solenoid_on;



/********************************************************************************/
/*										*/
/*	Solenoid Access methods 						*/
/*										*/
/********************************************************************************/

void queueSolenoid(int which)
{
   long bit = 1 << which;
   if ((solenoid_queued & bit) != 0) return;
   solenoid_queued |= bit;
   solenoid_queue[solenoid_end] = which;
   solenoid_end = (solenoid_end + 1) % NUM_SOLENOID;	
}


void removeAllSolenoids()
{
   solenoid_start = solenoid_end = 0;	
   solenoid_queued = 0;
   turnOffSolenoid();
}




/********************************************************************************/
/*										*/
/*	Solenoid setup methods							*/
/*										*/
/********************************************************************************/

void solenoidsSetup()
{
   pinMode(SOLENOID_PIN_DRIVER,OUTPUT);
   pinMode(SOLENOID_PIN_SELECT0,OUTPUT);
   pinMode(SOLENOID_PIN_SELECT1,OUTPUT);
   pinMode(SOLENOID_PIN_SELECT2,OUTPUT);
   pinMode(SOLENOID_PIN_SELECT3,OUTPUT);

   digitalWrite(SOLENOID_PIN_DRIVER,LOW);
   digitalWrite(SOLENOID_PIN_SELECT0,LOW);
   digitalWrite(SOLENOID_PIN_SELECT1,LOW);
   digitalWrite(SOLENOID_PIN_SELECT2,LOW);
   digitalWrite(SOLENOID_PIN_SELECT3,LOW);

   solenoid_start = 0;
   solenoid_end = 0;
   solenoid_queued = 0;
   next_solenoid = 0;
   solenoid_on = false;
}



/********************************************************************************/
/*										*/
/*	Solenoid time wrap methods						*/
/*										*/
/********************************************************************************/

void solenoidsWrap()
{
   next_solenoid = 0;
}




/********************************************************************************/
/*										*/
/*	Solenoid processing methods						*/
/*										*/
/********************************************************************************/

void solenoidsUpdate(unsigned long now)
{
   if (now >= next_solenoid && solenoid_on) {
      turnOffSolenoid();
      next_solenoid = addTime(now,SOLENOID_OFF_TIME);
    }
   if (now >= next_solenoid) {
      if (solenoid_start == solenoid_end) {		// empty queue
	 next_solenoid = addTime(now,SOLENOID_CHECK_TIME);
       }
      else {
	 turnOnSolenoid();
	 next_solenoid = addTime(now,SOLENOID_ON_TIME);
       }
    }
}



static void turnOffSolenoid()
{
   digitalWrite(SOLENOID_PIN_DRIVER,LOW);
   solenoid_on = false;
}


static void turnOnSolenoid()
{
   int which = solenoid_queue[solenoid_start];
   solenoid_start = (solenoid_start + 1) % NUM_SOLENOID;
   int bit = 1 << which;

   digitalWrite(SOLENOID_PIN_SELECT0,((which & 0x1) != 0) ? HIGH : LOW);
   digitalWrite(SOLENOID_PIN_SELECT1,((which & 0x2) != 0) ? HIGH : LOW);
   digitalWrite(SOLENOID_PIN_SELECT2,((which & 0x4) != 0) ? HIGH : LOW);
   digitalWrite(SOLENOID_PIN_SELECT3,((which & 0x8) != 0) ? HIGH : LOW);
   digitalWrite(SOLENOID_PIN_DRIVER,HIGH);

   solenoid_queued &= ~bit;
   solenoid_on = true;
}



/* end of solenoids.ino */

