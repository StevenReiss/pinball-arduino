


const int GROUND_PIN = 4;
const int CLOCK_PIN = 5;
const int LATCH_PIN = 6;
const int DATA_PIN = 7;
const int DIGIT3_PIN = 8;
const int DIGIT2_PIN = 9;
const int DIGIT1_PIN = 10;
const int POWER_PIN = 11;


const int digit_pins[] = {
  DIGIT1_PIN, DIGIT2_PIN, DIGIT3_PIN
};

const int NUM_DISPLAY = 6;

unsigned long  display_value[NUM_DISPLAY];
unsigned long  bit_values[NUM_DISPLAY][3];   

long next_update;
long next_set;
long next_count;
long last_count;

int set_count;

const int COUNT_INCR = 250;   // increment value every k ms

const int SET_INCR = 4;        // ms to keep a digit on

const int UPDATE_INCR = 100;   // update digit tables every k ms

const long MAX_VALUE = 1000000;

const int DIGIT_SHOW = 0;
const int DIGIT_HIDE = 1;

const unsigned int bits [] = {
  0xd7,       // 0
  0x50,       // 1  0x50
  0x67,       // 2
  0x75,       // 3
  0xf0,       // 4
  0xb5,       // 5
  0xb7,       // 6
  0x51,       // 7
  0xf7,       // 8
  0xf1        // 9
};

const unsigned int COMMA = 0x8;

const boolean debug_bits = false;
const boolean debug_output = false;


void setup() {
  Serial.begin(57600);

  pinMode(GROUND_PIN,OUTPUT);
  pinMode(POWER_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(DIGIT1_PIN,OUTPUT);
  pinMode(DIGIT2_PIN,OUTPUT);
  pinMode(DIGIT3_PIN,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(GROUND_PIN,LOW);
  digitalWrite(POWER_PIN,HIGH);
  digitalWrite(DATA_PIN,0);
  digitalWrite(CLOCK_PIN,0);
  digitalWrite(LATCH_PIN,0);
  digitalWrite(DIGIT1_PIN,DIGIT_HIDE);
  digitalWrite(DIGIT2_PIN,DIGIT_HIDE);
  digitalWrite(DIGIT3_PIN,DIGIT_HIDE);

  for (int i = 0; i < 6; ++i) {
     display_value[i] = 0;
  }
  updateTask();
  
  next_update = 0;
  next_set = 0;
  next_count = 0;
  set_count = 0;
  last_count = millis();
}

void loop() {
  long now = millis();
  
  if (now < last_count) {   // handle time wrap
    next_update = 0;
    next_set = 0;
    next_count = 0;
  }
  last_count = now;
  
  if (now >= next_count) {
    countTask();
    next_count = now + COUNT_INCR;
  }
  
  if (now >= next_update) {
     updateTask();
     next_update = now + UPDATE_INCR;
  }

  if (now >= next_set) {
    setTask();
    next_set = now + SET_INCR;
  }
}



void countTask()
{
  for (int i = 0; i < NUM_DISPLAY; ++i) {
     display_value[i] += i+1;
     if (display_value[i] >= 1000000) display_value[i] = display_value[i] % 1000000;
  }
}



void updateTask()
{
  int digits[10];
  for (int i = 0; i < NUM_DISPLAY; ++i) {
    long v = display_value[i];
    int j = 0;
    while (v > 0) {
      digits[j++] = v % 10;
      v = v / 10;
    }
    if (debug_bits) {
      Serial.print("VAL ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(display_value[i]);
      Serial.print(" ");
      Serial.print(j);
      for (int k = 0; k < j; ++k) {
        Serial.print(" ");
        Serial.print(digits[k]);
      }
      Serial.println();
    }
    if (j == 0) digits[j++] = 0;
    for (int k = 0; k < 3; ++k) {
      unsigned int v0 = 0;
      if (k < j) v0 = bits[digits[k]];
      unsigned int v1 = 0;
      if (k+3 < j) {
         v1 = bits[digits[k+3]];
         if (k == 0) v1 |= COMMA;
      }
      bit_values[i][k] = (v1<<8) | v0;
      if (debug_bits) {
        Serial.print("BIT ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(k);
        Serial.print(" ");
        Serial.print(bit_values[i][k]);
        Serial.println();
      }
    }
  }
}


void setTask()
{
  int idx = set_count++;
  if (set_count >= 3) set_count = 0;
  int oidx = idx+2;
  if (oidx >= 3) oidx = oidx-3;

  digitalWrite(LATCH_PIN,0);
  for (int j = 0; j < NUM_DISPLAY; ++j) {
    digitalWrite(DATA_PIN,0);
    digitalWrite(CLOCK_PIN,0);
    
    // select MPX to control CLOCK_PIN output
    int clockval = (j == 0 ? 1 : 0);
    // if MPX is used, clockval = 1, but mpx bits determine where it goes
    digitalWrite(13,clockval);

    long val = bit_values[j][idx];
    for (int i = 15; i >= 0; --i) {
      digitalWrite(CLOCK_PIN,0);
      int state = ((val & (1<<i)) != 0) ? 1 : 0;
      // state = 1;
      if (debug_output && j == 0) Serial.print(state);
      digitalWrite(DATA_PIN,state);
      digitalWrite(CLOCK_PIN,clockval);    // used on turn on of this
      digitalWrite(DATA_PIN,0);
    }
    digitalWrite(CLOCK_PIN,0);
   }

  digitalWrite(digit_pins[oidx],DIGIT_HIDE);
  digitalWrite(LATCH_PIN,1);    // transfer data to storage register
  digitalWrite(digit_pins[idx],DIGIT_SHOW);
  // digitalWrite(LATCH_PIN,0);

  if (debug_output) Serial.println();
}

