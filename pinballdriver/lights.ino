/********************************************************************************/
/*										*/
/*	lights.ino -- control the lights					*/
/*										*/
/********************************************************************************/



/********************************************************************************/
/*										*/
/*	Private storage 							*/
/*										*/
/********************************************************************************/

static bool lights[64];
static int  start_row;
static int  end_row;
static int  pulse_row;

static unsigned long next_light_pulse;		      // next time to pulse lights
static unsigned long next_light_rowset; 	      // next time to change rows



/********************************************************************************/
/*										*/
/*	Light access methods							*/
/*										*/
/********************************************************************************/

void setLight(int lno,bool fg)
{
   lights[lno] = fg;	
}


void lightOn(int lno)
{
   setLight(lno,true);
}


void lightOff(int lno)
{
   setLight(lno,false);
}


void disableAllLights()
{
   for (int i = 0; i < NUM_LIGHTS; ++i) lights[i] = false;
}


bool isLightEnabled(int lno)
{
   return lights[lno];
}



/********************************************************************************/
/*										*/
/*	Light setup methods							*/
/*										*/
/********************************************************************************/

void lightsSetup()
{
   pinMode(LIGHT_PIN_SELECT0,OUTPUT);
   pinMode(LIGHT_PIN_SELECT1,OUTPUT);
   pinMode(LIGHT_PIN_SELECT2,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE0,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE1,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE2,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE3,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE4,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE5,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE6,OUTPUT);
   pinMode(LIGHT_PIN_DRIVE7,OUTPUT);
   pinMode(LIGHT_PIN_ENABLE,OUTPUT);

   digitalWrite(LIGHT_PIN_SELECT0,LOW);
   digitalWrite(LIGHT_PIN_SELECT1,LOW);
   digitalWrite(LIGHT_PIN_SELECT2,LOW);
   digitalWrite(LIGHT_PIN_DRIVE0,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE1,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE2,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE3,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE4,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE5,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE6,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE7,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_ENABLE,LOW);

   for (int i = 0; i < NUM_LIGHTS; ++i) lights[i] = false;
   start_row = 0;
   end_row = LIGHT_NUM_ENABLED_ROW;
   next_light_pulse = 0;
   next_light_rowset = 0;
   pulse_row = 0;
}



/********************************************************************************/
/*										*/
/*	Light time wrap methods 						*/
/*										*/
/********************************************************************************/

void lightsWrap()
{
   next_light_pulse = 0;
   next_light_rowset = 0;
}



/********************************************************************************/
/*										*/
/*	Light action methods							*/
/*										*/
/********************************************************************************/

void lightsUpdate(unsigned long now)
{
   if (now >= next_light_rowset) {
      updateRows();
      next_light_rowset = addTime(now,LIGHT_CYCLE_TIME);
    }
   if (now >= next_light_pulse) {
      updatePulse();
      next_light_pulse = addTime(now,LIGHT_PULSE_TIME);
    }
}




/********************************************************************************/
/*										*/
/*	Reset methods								*/
/*										*/
/********************************************************************************/

void lightsReset()
{
   digitalWrite(LIGHT_PIN_ENABLE,LOW);
   digitalWrite(LIGHT_PIN_DRIVE0,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE1,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE2,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE3,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE4,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE5,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE6,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_DRIVE7,LIGHT_DRIVE_OFF);
   digitalWrite(LIGHT_PIN_ENABLE,LIGHT_DRIVE_OFF);
}




/********************************************************************************/
/*										*/
/*	Move to next set of rows						*/
/*										*/
/********************************************************************************/

static void updateRows()
{
   start_row = (start_row + 1) % NUM_LIGHT_ROWS;
   end_row = (end_row + 1) % NUM_LIGHT_ROWS;
}



static void updatePulse()
{
   int row = start_row + pulse_row;
   pulse_row = (pulse_row + 1) % LIGHT_NUM_ENABLED_ROW;

   int base = row * 8;
   digitalWrite(LIGHT_PIN_ENABLE,LOW);

   writeBit(LIGHT_PIN_SELECT0,row,0);
   writeBit(LIGHT_PIN_SELECT1,row,1);
   writeBit(LIGHT_PIN_SELECT2,row,2);

   for (int j = 0; j < NUM_LIGHT_COLUMNS; ++j) {
      if (lights[base+j]) digitalWrite(LIGHT_PIN_DRIVE(j),LIGHT_DRIVE_ON);
      else digitalWrite(LIGHT_PIN_DRIVE(j),LIGHT_DRIVE_OFF);
    }

   digitalWrite(LIGHT_PIN_ENABLE,HIGH);
}




/* end of lights.ino */
