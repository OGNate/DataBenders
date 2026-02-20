#include <Servo.h>

// Servo on Channel 5
const int CH5_PIN = 22;
Servo servo5;
const int SERVO5_PIN = 6; // Changed from 5

// Third motor - Channel 1
const int CH1_PIN = 42;      // Changed from 40
const int MOTOR3_INA = 43;   // Changed from 41
const int MOTOR3_INB = 44;   // Changed from 42
const int MOTOR3_ENA = 8;    // Stays the same

// Left track - Channel 3 (left joystick up/down)
const int CH3_PIN = 30;
const int LEFT_INA = 31;
const int LEFT_INB = 32;
const int LEFT_ENA = 2;

// Right track - Channel 2 (right joystick up/down)
const int CH2_PIN = 36;   // Changed from 3 (receiver channel 2)
const int RIGHT_INC = 37;  // Changed from 7
const int RIGHT_IND = 38;  // Changed from 8
const int RIGHT_ENB = 4;   // Changed from 11 

void setup() {
  Serial.begin(9600);

  // Servo setup
  servo5.attach(SERVO5_PIN);
  pinMode(CH5_PIN, INPUT);  
  
  // Left motor pins
  pinMode(CH3_PIN, INPUT);
  pinMode(LEFT_INA, OUTPUT);
  pinMode(LEFT_INB, OUTPUT);
  pinMode(LEFT_ENA, OUTPUT);
  
  // // Right motor pins
  pinMode(CH2_PIN, INPUT);
  pinMode(RIGHT_INC, OUTPUT);
  pinMode(RIGHT_IND, OUTPUT);
  pinMode(RIGHT_ENB, OUTPUT);

  // Third motor pins
  pinMode(CH1_PIN, INPUT);
  pinMode(MOTOR3_INA, OUTPUT);
  pinMode(MOTOR3_INB, OUTPUT);
  pinMode(MOTOR3_ENA, OUTPUT);  
}

void loop() {

  unsigned long ch5Pulse = pulseIn(CH5_PIN, HIGH, 25000);
  if (ch5Pulse > 0) {
    // Add deadband - only move if signal changes significantly
    static int lastAngle = 90;  // Remember last position
    int servoAngle = map(ch5Pulse, 1000, 2000, 20, 160);
    servoAngle = constrain(servoAngle, 0, 180);
    
    // Only update if change is > 2 degrees
    if (abs(servoAngle - lastAngle) > 2) {
      servo5.write(servoAngle);
      lastAngle = servoAngle;
    }
  }

  // Read left joystick (Channel 3)
  unsigned long ch3Pulse = pulseIn(CH3_PIN, HIGH, 25000);
  
  // Read right joystick (Channel 2)
  unsigned long ch2Pulse = pulseIn(CH2_PIN, HIGH, 25000);

  // Read Channel 1 for third motor
  unsigned long ch1Pulse = pulseIn(CH1_PIN, HIGH, 25000);  
  
  // Control left motor
  if (ch3Pulse > 0) {
    controlMotor(ch3Pulse, LEFT_INA, LEFT_INB, LEFT_ENA, "LEFT");
  }
  
  // Control right motor
  if (ch2Pulse > 0) {
    controlMotor(ch2Pulse, RIGHT_INC, RIGHT_IND, RIGHT_ENB, "RIGHT");
  }

  // Control third motor
  if (ch1Pulse > 0) {
    controlMotor(ch1Pulse, MOTOR3_INA, MOTOR3_INB, MOTOR3_ENA, "MOTOR3");
  }  
  
  delay(20);
}

void controlMotor(unsigned long pulseWidth, int pinA, int pinB, int pinEnable, String motorName) {
  int speed = 0;
  bool forward = true;
  
  if (pulseWidth > 1550) {  // Stick UP - Forward
    speed = map(pulseWidth, 1550, 2000, 0, 255);
    forward = true;
  } else if (pulseWidth < 1450) {  // Stick DOWN - Reverse
    speed = map(pulseWidth, 1450, 900, 0, 255);
    forward = false;
  } else {  // Deadzone - Stop
    speed = 0;
  }
  
  // Set direction
  digitalWrite(pinA, forward ? HIGH : LOW);
  digitalWrite(pinB, forward ? LOW : HIGH);
  
  // Set speed
  analogWrite(pinEnable, speed);
  
  // Debug output
  Serial.print(motorName);
  Serial.print(" - PWM: ");
  Serial.print(pulseWidth);
  Serial.print(" | Speed: ");
  Serial.print(speed);
  Serial.print(" | Dir: ");
  Serial.println(forward ? "FWD" : "REV");
}