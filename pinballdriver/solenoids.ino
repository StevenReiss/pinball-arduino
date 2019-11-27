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

static int solenoid_time[NUM_SOLENOID] = {     // multiples of basic unit (10ms)
	8,		// BALL_RELEASE
	3,		// LEFT_SHOOTER
	6,		// LA_DROP_RESET
	8,		// SER_DROP_RESET
	3,		// EJECT HOLE
	6,		// BA_DROP_RESET
	6,		// LL_DROP_RESET
	3,		// RIGHT_SHOOTER
	1,		// CREDIT_KNOCKER
	1,		// LASER_LAMPS
	1		// COIN_LOCKOUT
};


/********************************************************************************/
/*										*/
/*	Forward Definitions							*/
/*										*/
/********************************************************************************/

static void turnOffSolenoid(void);
static int turnOnSolenoid(void);




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

   digitalWrite(SOLENOID_PIN_DRIVER,SOLENOID_DRIVER_OFF);
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

static void turnOffSolenoid()
{
   digitalWrite(SOLENOID_PIN_DRIVER,SOLENOID_DRIVER_OFF);
   solenoid_on = false;
//   Serial.print("SOL OFF ");
//   Serial.println(micros());
}


static int turnOnSolenoid()
{
   int which0 = solenoid_queue[solenoid_start];
   solenoid_start = (solenoid_start + 1) % NUM_SOLENOID;
   int bit = 1 << which0;
   int which = which0 + FIRST_SOLENOID;
   writeBit(SOLENOID_PIN_SELECT0,which,0);
   writeBit(SOLENOID_PIN_SELECT1,which,1);
   writeBit(SOLENOID_PIN_SELECT2,which,2);
   writeBit(SOLENOID_PIN_SELECT3,which,3);
   digitalWrite(SOLENOID_PIN_DRIVER,SOLENOID_DRIVER_ON);

   solenoid_queued &= ~bit;
   solenoid_on = true;

   return which0;
}



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
	 int which = turnOnSolenoid();
	 unsigned long ontime = SOLENOID_ON_TIME * solenoid_time[which];
//	   Serial.print("SOLENOID ON ");
//	   Serial.print(which);
//	   Serial.print(" ");
//	   Serial.print(micros());
//	   Serial.print(" ");
//	   Serial.println(ontime);
	 next_solenoid = addTime(now,ontime);
       }
    }
}




/********************************************************************************/
/*										*/
/*	Reset methods								*/
/*										*/
/********************************************************************************/

void solenoidsReset()
{
   removeAllSolenoids();
}



/* end of solenoids.ino */
