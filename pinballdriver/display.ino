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
static byte display_digits[NUM_DISPLAY][8];
static byte display_digit_count[NUM_DISPLAY];

static int	display_left;
static byte display_left_digits[8];
static byte display_left_digit_count;

static int	display_right;
static byte display_right_digits[8];
static byte display_right_digit_count;

static bool need_digits[NUM_DISPLAY];

static unsigned long bit_values[NUM_DISPLAY][3];

static unsigned long next_display_update;
static unsigned long next_display_set;

static int set_count;
static int work_display;



/********************************************************************************/
/*										*/
/*	Display access methods							*/
/*										*/
/********************************************************************************/


void setDisplay(int i,long value)
{
   if (value >= MAX_VALUE) value = value % MAX_VALUE;
   display_value[i] = value;
   need_digits[i] = true;
}


void setDisplayLeft(int i)
{
   if (i >= MAX_HALF_VALUE) i = i % MAX_HALF_VALUE;
   display_left = i;
   need_digits[DISPLAY_SPLIT] = true;
}


void setDisplayRight(int i)
{
   if (i >= MAX_HALF_VALUE) i = i % MAX_HALF_VALUE;
   display_right = i;
   need_digits[DISPLAY_SPLIT] = true;
}


void blankDisplay(int i)
{
   display_value[i] = -1;
   need_digits[i] = true;
}


void blankDisplayLeft()
{
   display_left = -1;
   need_digits[DISPLAY_SPLIT] = true;
}


void blankDisplayRight()
{
   display_right = -1;
   need_digits[DISPLAY_SPLIT] = true;
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

   digitalWrite(DISPLAY_DATA_PIN,LOW);
   digitalWrite(DISPLAY_CLOCK_PIN,HIGH);
   digitalWrite(DISPLAY_LATCH_PIN,HIGH);
   digitalWrite(DISPLAY_CLOCK_MPX0_PIN,LOW);
   digitalWrite(DISPLAY_CLOCK_MPX1_PIN,LOW);
   digitalWrite(DISPLAY_CLOCK_MPX2_PIN,LOW);
   digitalWrite(DISPLAY_DIGIT1_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT2_PIN,DIGIT_HIDE);
   digitalWrite(DISPLAY_DIGIT3_PIN,DIGIT_HIDE);

   for (int i = 0; i < NUM_DISPLAY; ++i) {
      need_digits[i] = true;
      blankDisplay(i);
    }
   blankDisplayLeft();
   blankDisplayRight();

   updateTask();

   next_display_update = 0;
   next_display_set = 0;
   set_count = 0;
   work_display = 0;
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
   digitalWrite(DISPLAY_CLOCK_PIN,HIGH);
   digitalWrite(DISPLAY_LATCH_PIN,HIGH);
}



/********************************************************************************/
/*										*/
/*	Update task -- update bits based on values				*/
/*										*/
/********************************************************************************/

void updateTask()
{
   for (int i = 0; i < NUM_DISPLAY; ++i) {
      if (i == DISPLAY_SPLIT) continue;
      if (need_digits[i]) {
	 getDisplayDigits(i);
	 need_digits[i] = false;
       }
      long v = display_value[i];
      if (v < 0) {
	 bit_values[i][0] = 0;
	 bit_values[i][1] = 0;
	 bit_values[i][2] = 0;
	 continue;
       }
      int j = display_digit_count[i];
      byte * digits = display_digits[i];
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

   if (need_digits[DISPLAY_SPLIT]) {
      getSplitDisplayDigits();
      need_digits[DISPLAY_SPLIT] = false;
    }

   for (int k = 0; k < 3; ++k) {
      unsigned int v0 = 0;
      if (k < display_right_digit_count) v0 = bits[display_right_digits[k]];
      unsigned int v1 = 0;
      if (k < display_left_digit_count) v1 = bits[display_left_digits[k]];
      bit_values[DISPLAY_SPLIT][k] = (v1 << 8) | v0;
    }
}



static void getDisplayDigits(int dis)
{
   long v = display_value[dis];
   if (v < 0) {
      display_digit_count[dis] = 0;
    }
   else {
      display_digit_count[dis] = getDigits(v,display_digits[dis]);
    }
}



static void getSplitDisplayDigits()
{
   display_left_digit_count = getDigits(display_left,display_left_digits);
   display_right_digit_count = getDigits(display_right,display_right_digits);
//   for (int i = 0; i < 3; ++i) {
//    Serial.print(i);
//    Serial.print(" ");
//    Serial.print(display_left_digits[i]);
//    Serial.print(" ");
//    Serial.print(display_right_digits[i]);
//    Serial.print(" ");
//    Serial.print(display_left);
//    Serial.print(" ");
//    Serial.print(display_right);
//    Serial.println();
//   }
}



static byte getDigits(int v,byte digits[])
{
   if (v < 0) return 0;

   byte j = 0;
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
   ++work_display;
   if (work_display >= NUM_DISPLAY) {
      work_display = 0;
      ++set_count;
      if (set_count >= 3) set_count = 0;
    }
   int idx = set_count;
   int j = work_display + 1;

   int oidx = idx+2;
   if (oidx >= 3) oidx = oidx-3;

   digitalWrite(DISPLAY_DIGIT_PIN(oidx),DIGIT_HIDE);
   
   digitalWrite(DISPLAY_DATA_PIN,LOW);
   writeBit(DISPLAY_CLOCK_MPX0_PIN,j,0);
   writeBit(DISPLAY_CLOCK_MPX1_PIN,j,1);
   writeBit(DISPLAY_CLOCK_MPX2_PIN,j,2);
   digitalWrite(DISPLAY_LATCH_PIN,LOW);

   long val = bit_values[work_display][idx];
   for (int i = 15; i >= 0; --i) {
      digitalWrite(DISPLAY_CLOCK_PIN,LOW);
      if (work_display == 4) Serial.print(bitRead(val,i));
      writeBit(DISPLAY_DATA_PIN,val,i);
      digitalWrite(DISPLAY_CLOCK_PIN,HIGH);
      digitalWrite(DISPLAY_DATA_PIN,LOW);
    }
    if (work_display == 4) {
      Serial.print(" ");
      Serial.print(idx);
      Serial.print(" ");
      Serial.print(oidx);
      Serial.print(" ");
      Serial.println(val);
    }
   // digitalWrite(DISPLAY_CLOCK_PIN,LOW);

   digitalWrite(DISPLAY_DIGIT_PIN(oidx),DIGIT_HIDE);
   digitalWrite(DISPLAY_LATCH_PIN,HIGH);
   digitalWrite(DISPLAY_DIGIT_PIN(idx),DIGIT_SHOW);
}



/* end of display.ino */
