#include <LiquidCrystal.h>

#define enA 11  
#define in1 1  
#define in2 2  


#define enB 3 
#define in3 12 
#define in4 13 

// --- Sensor Pins ---
const int lineFollowRightPin = A1;
const int lineFollowLeftPin = A2;

const int encoderRightPin = A4;
const int encoderLeftPin = A5;

// --- Constants ---
#define FORWARD 1
#define BACKWARD 0

#define LEFTBLACKTHRES 300
#define LEFTWHITETHRES 700
#define RIGHTBLACKTHRES 500
#define RIGHTWHITETHRES 700

#define CM_PER_COUNT 5

// --- LCD Initialization ---
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// --- Global Variables ---
int state = 0;
unsigned long lastMillis;

volatile unsigned long rightCount = 0;
volatile unsigned long leftCount = 0;

int stopCounter = 0;


bool firstStopDone = false;


ISR(PCINT1_vect) {
  uint8_t portState = PINC;

  int rightState = (portState >> 1) & 1;
  int leftState  = (portState >> 2) & 1;

  static int lastRightState = 0;
  static int lastLeftState = 0;

  if (rightState != lastRightState) rightCount++;
  if (leftState != lastLeftState) leftCount++;

  lastRightState = rightState;
  lastLeftState = leftState;
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(lineFollowRightPin, INPUT);
  pinMode(lineFollowLeftPin, INPUT);
  pinMode(encoderRightPin, INPUT);
  pinMode(encoderLeftPin, INPUT);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT10);

  stopMotors();
  Serial.begin(115200);
  lastMillis = millis();
}

void rotateMotorA(int speed, int direction) {
  speed = constrain(speed, 0, 255);
  analogWrite(enA, speed);

  if (direction == FORWARD) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
}

void rotateMotorB(int speed, int direction) {
  speed = constrain(speed, 0, 255);
  analogWrite(enB, speed);

  if (direction == FORWARD) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {

  int leftVal = analogRead(lineFollowLeftPin);
  int rightVal = analogRead(lineFollowRightPin);

  // ============================================================
  //   NEW BLOCK RE-INSERTED:
  //   STOP FOREVER WHEN BOTH SENSORS SEE BLACK
  // ============================================================
  if (leftVal < 430 && rightVal < 430) {

    stopMotors();

    float avgCounts = (leftCount + rightCount) / 2.0;
    float distance = avgCounts * CM_PER_COUNT;

    unsigned long currentMillis = millis();
    unsigned long milliseconds = (currentMillis - lastMillis) % 1000;
    unsigned long seconds = (currentMillis - lastMillis) / 1000;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time:");
    lcd.setCursor(6,0);
    lcd.print(seconds);
    lcd.print(":");
    lcd.print(milliseconds);

    lcd.setCursor(0, 1);
    lcd.print("Dist:");
    lcd.print(distance);
    lcd.print(" cm");

    // Stop robot permanently at finish line
    while (1) stopMotors();
  }
  // ============================================================


  // --- Distance Calculation ---
  float avgCounts = (leftCount + rightCount) / 2.0;
  float distance = avgCounts * CM_PER_COUNT;

  // ---- 30 cm stop (one time only) ----
  if (!firstStopDone && distance >= 30.0) {
    stopMotors();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("30cm reached!");
    lcd.setCursor(0, 1);
    lcd.print("Stopping 2s...");
    
    delay(2000);
    firstStopDone = true;

    lcd.clear();
  }

  // --- LINE FOLLOWING ---
  if (state == 0) {
    rotateMotorA(255, BACKWARD);
    rotateMotorB(200, FORWARD);
  } else {
    rotateMotorA(180, FORWARD);
    rotateMotorB(200, BACKWARD);
  }

  if (leftVal < 500) state = 0;
  else if (rightVal < 500) state = 1;

  // --- LCD Display ---
  lcd.setCursor(0, 0);
  lcd.print("L:"); lcd.print(leftVal);
  lcd.setCursor(8, 0);
  lcd.print("R:"); lcd.print(rightVal);

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");

  lcd.setCursor(14, 1);
  lcd.print(state);
}
