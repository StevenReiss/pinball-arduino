/********************************************************************************/
/*										*/
/*	sounds.ino -- low level sound code					*/
/*										*/
/********************************************************************************/


/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static byte	sound_queue[SOUND_QUEUE_SIZE];
static byte	sound_start;
static byte	sound_end;

static unsigned long next_sound;
static bool	sound_on;

static volatile int play_counter;
static int	play_max;
static unsigned char * play_data;
static bool	play_on;



/********************************************************************************/
/*										*/
/*	Solenoid Access methods 						*/
/*										*/
/********************************************************************************/

void queueSound(int which)
{
   sound_queue[sound_end] = which;
   sound_end = (sound_end + 1) % SOUND_QUEUE_SIZE;
}




/********************************************************************************/
/*										*/
/*	Solenoid setup methods							*/
/*										*/
/********************************************************************************/

void soundsSetup()
{
   pinMode(SOUND_PIN_DRIVER,OUTPUT);
   pinMode(SOUND_PIN_SELECT0,OUTPUT);
   pinMode(SOUND_PIN_SELECT1,OUTPUT);
   pinMode(SOUND_PIN_SELECT2,OUTPUT);
   pinMode(SOUND_PIN_ALTERNATE_SET,OUTPUT);

   sound_start = 0;
   sound_end = 0;
   next_sound = 0;
   sound_on = false;

   digitalWrite(SOUND_PIN_DRIVER,SOUND_OFF);
   digitalWrite(SOUND_PIN_SELECT0,LOW);
   digitalWrite(SOUND_PIN_SELECT1,LOW);
   digitalWrite(SOUND_PIN_SELECT2,LOW);
   digitalWrite(SOUND_PIN_ALTERNATE_SET,LOW);

   soundPlaySetup();
}



void soundPlaySetup()
{
   play_counter = 0;
   play_max = 0;
   play_data = NULL;
   play_on = false;

   pinMode(SOUND_PLAY_PWM_PIN,OUTPUT);

   // setup timer 3 with fast PWM mode, noninverting
   // with timer clock prescaler = 1
   TCCR1A = 0;
   TCCR1B = 0;
   bitSet(TCCR1A,WGM21);
   bitSet(TCCR1A,WGM20);
   bitSet(TCCR1A,COM2A1);
   bitSet(TCCR1B,CS20);
   OCR1A = 0;
}



/********************************************************************************/
/*										*/
/*	Solenoid time wrap methods						*/
/*										*/
/********************************************************************************/

void soundsWrap()
{
   next_sound = 0;
}



/********************************************************************************/
/*										*/
/*	Sound processing methods						*/
/*										*/
/********************************************************************************/

void soundsReset()
{
   digitalWrite(SOUND_PIN_DRIVER,SOUND_OFF);
   fastpwmStop();
}


/********************************************************************************/
/*										*/
/*	Sound processing methods						*/
/*										*/
/********************************************************************************/

void soundsUpdate(unsigned long now)
{
   if (now >= next_sound && sound_on) {
      turnOffSound();
      next_sound = addTime(now,SOUND_OFF_TIME);
    }
   if (now >= next_sound) {
      if (sound_start == sound_end) {		  // empty queue
	 next_sound = addTime(now,SOUND_CHECK_TIME);
       }
      else {
	 turnOnSound();
	 next_sound = addTime(now,SOUND_ON_TIME);
       }
    }

   playCheck();
}



static void turnOffSound()
{
   digitalWrite(SOUND_PIN_DRIVER,LOW);
   sound_on = false;
}



static void turnOnSound()
{
   int which = sound_queue[sound_start];
   sound_start = (sound_start + 1) % SOUND_QUEUE_SIZE;

   if (which >= 5) {
      digitalWrite(SOUND_PIN_ALTERNATE_SET,HIGH);
      which -= 5;
    }
   else {
      digitalWrite(SOUND_PIN_ALTERNATE_SET,LOW);
    }

   digitalWrite(SOUND_PIN_SELECT0,((which & 0x1) != 0) ? HIGH : LOW);
   digitalWrite(SOUND_PIN_SELECT1,((which & 0x2) != 0) ? HIGH : LOW);
   digitalWrite(SOUND_PIN_SELECT2,((which & 0x4) != 0) ? HIGH : LOW);
   digitalWrite(SOUND_PIN_DRIVER,HIGH);

   sound_on = true;
}



/********************************************************************************/
/*										*/
/*	Sound play methods							*/
/*										*/
/********************************************************************************/

static void playCheck()
{
   if (play_on && play_data == NULL) {
      fastpwmStop();
    }
}


static void fastpwmStart()
{
   bitClear(TCCR1B,CS22);
   bitClear(TCCR1B,CS21);
   bitSet(TCCR1B,CS20);
   play_on = true;
}


static void fastpwmStop()
{
   bitClear(TCCR1B,CS22);
   bitClear(TCCR1B,CS21);
   bitClear(TCCR1B,CS20);
   play_on = false;
}


static void fastpwmPlay(unsigned char * data,int length)
{
   noInterrupts();
   play_data = data;
   play_max = length;
   play_counter = 0;
   interrupts();

   if (!play_on) fastpwmStart();
}



ISR(TIMER1_COMPA_vect)
{
   if (play_data == NULL) return;
   if (play_counter >= play_max) {
      play_data = NULL;
      return;
    }
   OCR1A = play_data[play_counter++];
}



/* end of sounds.ino */

