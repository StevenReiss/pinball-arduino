const unsigned int bits [] = {
  0, 0x84, 0x8c, 0xd6, 0xb4, 0x76, 0x7e, 0xc4, 0xfc, 0xf4, 0xec
};

const int BITSIZE = 11;

const int DATA_PIN = 3;
const int CLOCK_PIN = 5;
const int LATCH_PIN = 4;
const int SET0 = 10;
const int NUM_SETS = 8;
const int NUM_ITEMS = 6;

const int ON_TIME = 4;
const int UPDATE_TIME = 2000;

int cur_value = 0;
int cur_output = 0;

long next_update = 0;
long next_set = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(LATCH_PIN,OUTPUT);
  for (int i = 0; i < NUM_SETS; ++i) {
     if (i >= 4) continue;
     pinMode(SET0+i,OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  long now = millis();
  if (now >= next_update) {
    cur_value = (cur_value + 1) % BITSIZE;
    next_update = now + UPDATE_TIME;
  }

  if (now >= next_set) {
    if (cur_output < 4)
       digitalWrite(SET0 + cur_output,1);
    cur_output = (cur_output + 1)%NUM_SETS;
    setValue();
    next_set = now + ON_TIME;
    if (cur_output < 4) 
       digitalWrite(SET0 + cur_output,0);
  }
}



void setValue()
{
  digitalWrite(LATCH_PIN,0);

  for (int j = 0; j < NUM_ITEMS; ++j) {
    digitalWrite(DATA_PIN,0);
    digitalWrite(CLOCK_PIN,0);
    int val = bits[cur_value];
    /******
    Serial.print("OUTPUT ");
    Serial.println(val);
    *******/
    for (int i = 7; i >= 0; --i) {
      digitalWrite(CLOCK_PIN,0);
      int state = (val & (1<<i)) ? 1 : 0;
      digitalWrite(DATA_PIN,state);
      digitalWrite(CLOCK_PIN,1);
      delay(1);
      digitalWrite(DATA_PIN,0);
    }
    digitalWrite(CLOCK_PIN,0);
  }

  digitalWrite(LATCH_PIN,1);
}

