
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial myBT(A4, A5); 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// ----------MOTOR PINS----------
// Left Motor
#define enA 11  
#define in1 1   
#define in2 2

// Right Motor
#define enB 3   
#define in3 12
#define in4 13 

// ----------SETTINGS----------
int carSpeed = 150; 
char command;       
void setup() {
  
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows
  lcd.clear();
  myBT.begin(9600); // Start Bluetooth communication

  // Set all motor pins to Output
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  stopCar();
  
  delay(2000); 
}

// ------------MAIN LOOP------------
void loop() {
  // Check if phone sent a command
  if (myBT.available()) {
    command = myBT.read();
    
    // Check which letter was sent and move accordingly
    if (command == 'F') {
      forward();
      lcd.setCursor(0,0);
      lcd.print("forawrd");
    }
    else if (command == 'B') {
      backward();
      lcd.setCursor(0,0);
      lcd.print("backd");
    }
    else if (command == 'L') {
      left();
      lcd.setCursor(0,0);
      lcd.print("left");
    }
    else if (command == 'R') {
      right();
      lcd.setCursor(0,0);
      lcd.print("right");
    }
    else if (command == 'S' || command == '0') {
      stopCar();
      lcd.setCursor(0,0);
      lcd.print("stopt");
    }
  }
}

// ------------MOTOR FUNCTIONS------------

void forward() {
  // Left Motor Forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, carSpeed);

  // Right Motor Forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, carSpeed);
}

void backward() {
  // Left Motor Backward
  digitalWrite(in1, LOW);
NEW SKETCH

  digitalWrite(in2, HIGH);
  analogWrite(enA, carSpeed);

  // Right Motor Backward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, carSpeed);
}

void left() {
  // TANK TURN LEFT: Left wheels back, Right wheels forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, carSpeed);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, carSpeed);   
  
}

void right() {
  // TANK TURN RIGHT: Left wheels forward, Right wheels back
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, carSpeed);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, carSpeed);
}

void stopCar() {
  // Turn everything OFF
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}