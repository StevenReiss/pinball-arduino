/********************************************************************************/
/*										*/
/*	switches.ino -- low level switch handling code				*/
/*										*/
/********************************************************************************/



/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static bool	switch_value[NUM_SWITCH];		 // current value
static byte	switch_count[NUM_SWITCH];		 // # times at that value

static unsigned long next_switch_read;
static unsigned long next_switch_select;
static int switch_row;

const byte SWITCH_KNOWN = 100;



/********************************************************************************/
/*										*/
/*	Switch access methods							*/
/*										*/
/********************************************************************************/

bool getSwitch(int ino)
{
   if (switch_count[ino] == SWITCH_KNOWN) return switch_value[ino];
   return !switch_value[ino];
}


void handleSwitchChanges(int max,SwitchCallback off,SwitchCallback on)
{
   int ct = 0;
   for (int i = 0; i < NUM_SWITCH; ++i) {
      if (switch_count[i] == SWITCH_BOUNCE_CYCLES) {
	 if (switch_value[i]) {
	    (*on)(i);
	  }
	 else {
	    (*off)(i);
	  }
	 switch_count[i] = SWITCH_KNOWN;
	 if (ct++ >= max) break;
       }
    }
}



/********************************************************************************/
/*										*/
/*	Switch setup methods m							*/
/*										*/
/********************************************************************************/

void switchesSetup()
{
   pinMode(SWITCH_PIN_SELECT0,OUTPUT);
   pinMode(SWITCH_PIN_SELECT1,OUTPUT);
   pinMode(SWITCH_PIN_SELECT2,OUTPUT);

   pinMode(SWITCH_PIN_READ0,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ1,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ2,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ3,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ4,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ5,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ6,INPUT_PULLUP);
   pinMode(SWITCH_PIN_READ7,INPUT_PULLUP);

   digitalWrite(SWITCH_PIN_SELECT0,LOW);
   digitalWrite(SWITCH_PIN_SELECT1,LOW);
   digitalWrite(SWITCH_PIN_SELECT2,LOW);

   for (int i = 0; i < NUM_SWITCH; ++i) {
      switch_value[i] = false;
      switch_count[i] = 0;		// get initial value of switches
    }

   next_switch_read = 0;
   next_switch_select = 0;
   switch_row = 0;
}



/********************************************************************************/
/*										*/
/*	Switch wrap methods							*/
/*										*/
/********************************************************************************/

void switchesWrap()
{
   next_switch_read = 0;
   next_switch_select = 0;
}



/********************************************************************************/
/*										*/
/*	Switch loop  methods							*/
/*										*/
/********************************************************************************/

void switchesUpdate(unsigned long now)
{
   if (next_switch_read == 0 && now >= next_switch_select) {
      handleSelect();
      next_switch_read = addTime(now,SWITCH_DELAY_TIME);
      next_switch_select = addTime(now,SWITCH_CHECK_TIME);
    }
   if (next_switch_read > 0 && now >= next_switch_read) {
      handleRead();
      next_switch_read = 0;
      next_switch_select = addTime(now,SWITCH_CHECK_TIME);
    }
}


void switchesReset()
{

}


/********************************************************************************/
/*										  */
/*	Switch action methods							*/
/*										*/
/********************************************************************************/


static void handleSelect()
{
   switch_row = (switch_row + 1) % NUM_SWITCH_ROWS;
   writeBit(SWITCH_PIN_SELECT0,switch_row,0);
   writeBit(SWITCH_PIN_SELECT1,switch_row,1);
   writeBit(SWITCH_PIN_SELECT2,switch_row,2);

}


static void handleRead()
{
   int base = switch_row;
   for (int i = 0; i < NUM_SWITCH_ROWS; ++i) {
      int v = digitalRead(SWITCH_PIN_READ(i));
      bool val = (v == SWITCH_ON ? true : false);
      int swno = base*8 + i;
      if (switch_value[swno] == val) {
	 if (switch_count[swno] != SWITCH_KNOWN) {
	    if (switch_count[swno] < SWITCH_BOUNCE_CYCLES) ++switch_count[swno];
	  }
       }
      else {
	 Serial.print("SWITCH CHANGE ");
	 Serial.print(base);
	 Serial.print(" ");
	 Serial.print(i);
	 Serial.print(" ");
	 Serial.print(val);
	 Serial.println();
	 switch_value[swno] = val;
	 switch_count[swno] = 1;
       }
    }
}



/* end of switches.ino */












