// --- PIN DEFINITIONS ---
// Left Motor Pins
const int ENA = 9;   
const int IN1 = 8;  
const int IN2 = 7;  

// Right Motor Pins
const int ENB = 10;  
const int IN3 = 6;   
const int IN4 = 5;   

// Motor Speed (0-255)
const int motorSpeed = 150;

// Flag to ensure the action only runs once in the loop
bool executed = false;

// Function Prototypes
void moveForward();
void stopMotors();

// --- SETUP FUNCTION ---
void setup() {
  // Initialize all control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize Serial communication for debugging (optional)
  Serial.begin(9600);
  Serial.println("Vehicle Control Initialized. Waiting to start...");
}


// --- MAIN LOOP ---
void loop() {
  // Run the sequence only once
  if (!executed) {
    Serial.println("Starting movement: Forward for 10 seconds...");

    // 1. Start moving forward
    moveForward();

    // 2. Wait for 10 seconds (10,000 milliseconds)
    delay(10000);

    // 3. Stop the motors
    Serial.println("Stopping motors.");
    stopMotors();

    // Set the flag so this block doesn't run again
    executed = true;
  }
}

// --- MOVEMENT FUNCTIONS ---
// Function to set both motors to move forward
void moveForward() {
  // Set Left Motor direction (Forward)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, motorSpeed); // Set Left Motor speed

  // Set Right Motor direction (Forward)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, motorSpeed); // Set Right Motor speed

  Serial.print("Moving at speed: ");
  Serial.println(motorSpeed);
}

// Function to stop both motors instantly
void stopMotors() {
  // Set PWM speed to zero
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  

