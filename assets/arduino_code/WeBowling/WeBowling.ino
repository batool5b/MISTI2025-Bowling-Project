/*
 * Author: Batool Albonni
 * GitHub: @batool5b
 * Date: 2025-01-29
*/

// Include necessary libraries
#include <Stepper.h> // Arduino Stepper Library

// Pin definitions for motor control
const int ENA = 5; // PWM pin for motor A speed control
const int IN1 = 6;  // Direction pin 1 for motor A
const int IN2 = 7;  // Direction pin 2 for motor A
const int IN3 = 8;  // Direction pin 1 for motor B
const int IN4 = 9;  // Direction pin 2 for motor B+
const int ENB = 10;  // PWM pin for motor B speed control

// Analog joystick pins
const int X_PIN = A3;
const int Y_PIN = A4;

// Potentiometer pin for speed control
const int SPEED_CONTROL = A0;

// Stepper motor constants
const int STEPS_PER_REV = 255;
Stepper stepper_NEMA17(STEPS_PER_REV, 1, 2, 3, 4); // Initialize Stepper

// Threshold values
const int THRESHOLD_TURN_RIGHT = 4;
const int THRESHOLD_TURN_LEFT = 1000;
const int NEUTRAL_THRESHOLD_MIN = 450;
const int NEUTRAL_THRESHOLD_MAX = 550;
const int FORWARD_THRESHOLD = 6;

// Motor speed range
const int MIN_SPEED = 0;
const int MAX_SPEED = 80;

// Function to stop DC motors
void stopDCMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Function to drive motors forward
void driveForward(int speed) {
  analogWrite(ENA, speed); // Set motor A speed
  analogWrite(ENB, speed); // Set motor B speed
  digitalWrite(IN1, HIGH); // Motor A forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // Motor B forward
  digitalWrite(IN4, LOW);
}

// Function to stop the stepper motor
void stopStepperMotor() {
  // Fully de-energize the stepper motor coils
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
}

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(115200);

  // Set joystick pins as inputs
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);

  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Stop motors at the start
  stopDCMotors();
  stopStepperMotor();

  Serial.println("Setup complete.");
  delay(100); // Short delay for setup stabilization
}

void loop() {
  // Read joystick values
  int xData = analogRead(X_PIN);
  int yData = analogRead(Y_PIN);

  // Debug joystick data
  Serial.print("X-Data: ");
  Serial.print(xData);
  Serial.print("\tY-Data: ");
  Serial.print(yData);

  // Read potentiometer and map to motor speed
  int sensorReading = analogRead(SPEED_CONTROL);
  int motorSpeed = map(sensorReading, 0, 1023, MIN_SPEED, MAX_SPEED);

  // Control stepper motor based on joystick Y-axis
  if (yData < THRESHOLD_TURN_RIGHT) { // Turning right
    Serial.print(" | Action: Turning Right");
    if (motorSpeed > 0) {
      stepper_NEMA17.setSpeed(motorSpeed); // Set speed
      stepper_NEMA17.step(-STEPS_PER_REV); // Turn right
    }
  } else if (yData > THRESHOLD_TURN_LEFT) { // Turning left
    Serial.print(" | Action: Turning Left");
    if (motorSpeed > 0) {
      stepper_NEMA17.setSpeed(motorSpeed); // Set speed
      stepper_NEMA17.step(STEPS_PER_REV); // Turn left
    }
  } else if (xData < FORWARD_THRESHOLD) { // Moving forward
    Serial.print(" | Action: Moving Forward");
    driveForward(255); // Full speed forward
    delay(3000); // Move forward for 3 seconds
    stopDCMotors(); // Stop after moving forward
  } else if (xData >= NEUTRAL_THRESHOLD_MIN && xData <= NEUTRAL_THRESHOLD_MAX &&
             yData >= NEUTRAL_THRESHOLD_MIN && yData <= NEUTRAL_THRESHOLD_MAX) {
    // Joystick in neutral position
    Serial.print(" | Action: Neutral - Stopping Stepper Motor");
    stopStepperMotor(); // Stop the stepper motor
  } else {
    Serial.print(" | Action: Stopped");
    stopDCMotors(); // Stop all motors
    stopStepperMotor(); // Stop the stepper motor
  }

  Serial.println(""); // Newline for debug readability
  delay(100); // Short delay for loop stabilization
}
