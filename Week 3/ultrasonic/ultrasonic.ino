// ---------- Ultrasonic ----------
#define TRIG A4
#define ECHO A5

// ---------- Motor Driver Pins ----------
#define enA 11
#define enB 3
#define in1 1  
#define in2 2
#define in3 12
#define in4 13

// ---------- Motor Functions ----------
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// ---------- Get Distance ----------
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}

// ---------- Variables ----------
bool turning = false;
unsigned long turnStartTime = 0;

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, 200);
  analogWrite(enB, 200);
}

// ---------- MAIN LOOP ----------
void loop() {
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (!turning) {
    if (distance <= 20) {       // obstacle detected
      turning = true;
      turnStartTime = millis();
      turnLeft();
    } else {
      forward();
    }
  } else {
    turnLeft();                // keep turning
    if (millis() - turnStartTime >= 800) { // turn for 0.8 sec
      turning = false;          // done turning
      forward();                // resume forward
    }
  }

  delay(50);
}
