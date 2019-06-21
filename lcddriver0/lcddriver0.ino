
const int ON_TIME = 8;
const int OFF_TIME = 16;

long turn_off = 0;
long turn_on = 0;

int OUT_PIN = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_PIN,OUTPUT);
  digitalWrite(OUT_PIN,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  long now = millis();
  if (turn_on == 0 || now >= turn_on) {
    digitalWrite(OUT_PIN,1);
    turn_off = now + ON_TIME;
    turn_on = turn_off + OFF_TIME;
  }
  else if (now >= turn_off) {
    digitalWrite(OUT_PIN,0);
  }
}

