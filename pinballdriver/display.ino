/********************************************************************************/
/*										*/
/*	diplay.ino -- Manage the digit displays 				*/
/*										*/
/********************************************************************************/



/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static long display_value[NUM_DISPLAY];
static unsigned long bit_values[NUM_DISPLAY][3];

static int	display_left;
static int	display_right;

static unsigned long next_display_update;
static unsigned long next_display_set;

static int set_count;



/********************************************************************************/
/*										*/
/*	Display access methods							*/
/*										*/
/********************************************************************************/


void setDisplay(int i,long value)
{
   display_value[i] = value;
   if (display_value[i] >= MAX_VALUE) display_value[i] = display_value[i] % MAX_VALUE;
}


void setDisplayLeft(int i)
{
   if (i >= 1000) i = 999;
   display_left = i;
}


void setDisplayRight(int i)
{
   if (i >= 1000) i = 999;
   display_right = i;
}


void blankDisplay(int i)
{
   display_value[i] = -1;
}


void blankDisplayLeft()
{
   display_left = -1;
}


void blankDisplayRight()
{
   display_right = -1;
}



/********************************************************************************/
/*										*/
/*	Setup methods for the displays						*/
/*										*/
/********************************************************************************/

void displaySetup()
{
   pinMode(DISPLAY_DATA_PIN,OUTPUT);
   pinMode(DISPLAY_CLOCK_PIN,OUTPUT);
   pinMode(DISPLAY_CLOCK_MPX0_PIN,OUTPUT);
   pinMode(DISPLAY_CLOCK_MPX1_PIN,OUTPUT);
   pinMode(DISPLAY_CLOCK_MPX2_PIN,OUTPUT);
   pinMode(DISPLAY_LATCH_PIN,OUTPUT);
   pinMode(DISPLAY_DIGIT1_PIN,OUTPUT);
   pinMode(DISPLAY_DIGIT2_PIN,OUTPUT);
   pinMode(DISPLAY_DIGIT3_PIN,OUTPUT);

   digitalWrite(DISPLAY_DATA_PIN,0);
   digitalWrite(DISPLAY_CLOCK_PIN,0);
   digitalWrite(DISPLAY_LATCH_PIN,0);
   digitalWrite(DISPLAY_CLOCK_MPX0_PIN,0);
   digitalWrite(DISPLAY_CLOCK_MPX1_PIN,0);
   digitalWrite(DISPLAY_CLOCK_MPX2_PIN,0);
   digitalWrite(DISPLAY_DIGIT1_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT2_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT3_PIN,DIGIT_HIDE);

   for (int i = 0; i < NUM_DISPLAY; ++i) display_value[i] = 0;
   display_left = 0;
   display_right = 0;

   updateTask();

   next_display_update = 0;
   next_display_set = 0;
   set_count = 0;
}



/********************************************************************************/
/*										*/
/*	Time wrap methods for the displays					*/
/*										*/
/********************************************************************************/

void displayWrap()
{
   next_display_update = 0;
   next_display_set = 0;
}



/********************************************************************************/
/*										*/
/*	Processing methods for the displays					*/
/*										*/
/********************************************************************************/

void displayUpdate(unsigned long now)
{
   if (now >= next_display_update) {
      updateTask();
      next_display_update = addTime(now,DISPLAY_UPDATE_INCR);
    }

   if (now >= next_display_set) {
      setTask();
      next_display_set = addTime(now,DISPLAY_SET_INCR);
    }
}



/********************************************************************************/
/*										*/
/*	Reset methods								*/
/*										*/
/********************************************************************************/

void displayReset()
{
   digitalWrite(DISPLAY_DIGIT1_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT2_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT3_PIN,DIGIT_HIDE);
}



/********************************************************************************/
/*										*/
/*	Update task -- update bits based on values				*/
/*										*/
/********************************************************************************/

void updateTask()
{
   unsigned int digits[10],rdigits[10];

   for (int i = 0; i < NUM_DISPLAY; ++i) {
      if (i == DISPLAY_SPLIT) continue;
      long v = display_value[i];
      if (v < 0) {
	 bit_values[i][0] = 0;
	 bit_values[i][1] = 0;
	 bit_values[i][2] = 0;
	 continue;
       }
      int j = getDigits(v,digits);
      for (int k = 0; k < 3; ++k) {
	 unsigned int v0 = 0;
	 if (k < j) v0 = bits[digits[k]];
	 unsigned int v1 = 0;
	 if (k+3 < j) {
	    v1 = bits[digits[k+3]];
	    if (k == 0) v1 |= DISPLAY_COMMA;
	  }
	 bit_values[i][k] = (v1 << 8) | v0;
       }
    }

   int jleft = getDigits(display_left,digits);
   int jright = getDigits(display_right,rdigits);
   for (int k = 0; k < 3; ++k) {
      unsigned int v0 = 0;
      if (k < jright) v0 = bits[rdigits[k]];
      unsigned int v1 = 0;
      if (k < jleft) v1 = bits[digits[k]];
      bit_values[DISPLAY_SPLIT][k] = (v1 << 8) | v0;
    }
}



static int getDigits(int v,unsigned int digits[])
{
   if (v < 0) return 0;

   int j = 0;
   while (v > 0) {
      digits[j++] = v % 10;
      v = v / 10;
    }
   if (j == 0) digits[j++] = 0;

   return j;
}



/********************************************************************************/
/*										*/
/*	Set task								*/
/*										*/
/********************************************************************************/

void setTask()
{
   int idx = set_count++;
   if (set_count >= 3) set_count = 0;
   int oidx = idx+2;
   if (oidx >= 3) oidx = oidx-3;

   digitalWrite(DISPLAY_LATCH_PIN,0);
   for (int j = 0; j < NUM_DISPLAY; ++j) {
      digitalWrite(DISPLAY_DATA_PIN,LOW);
      digitalWrite(DISPLAY_CLOCK_PIN,LOW);
      digitalWrite(DISPLAY_CLOCK_MPX0_PIN,((j & 0x1) != 0 ? HIGH : LOW));
      digitalWrite(DISPLAY_CLOCK_MPX1_PIN,((j & 0x2) != 0 ? HIGH : LOW));
      digitalWrite(DISPLAY_CLOCK_MPX2_PIN,((j & 0x4) != 0 ? HIGH : LOW));

      long val = bit_values[j][idx];
      for (int i = 15; i >= 0; --i) {
	 digitalWrite(DISPLAY_CLOCK_PIN,LOW);
	 int state = ((val & (1<<i)) != 0) ? HIGH : LOW;
	 digitalWrite(DISPLAY_DATA_PIN,state);
	 digitalWrite(DISPLAY_CLOCK_PIN,HIGH);
	 digitalWrite(DISPLAY_DATA_PIN,LOW);
       }
      digitalWrite(DISPLAY_CLOCK_PIN,LOW);
    }

   digitalWrite(DISPLAY_DIGIT_PIN(oidx),DIGIT_HIDE);
   digitalWrite(DISPLAY_LATCH_PIN,HIGH);
   digitalWrite(DISPLAY_DIGIT_PIN(idx),DIGIT_SHOW);
}



/* end of display.ino */
