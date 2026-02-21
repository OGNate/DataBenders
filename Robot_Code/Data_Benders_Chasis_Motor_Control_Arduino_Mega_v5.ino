#include <Servo.h>

 

const int FORWARD_THRESHOLD = 1600;

const int REVERSE_THRESHOLD = 1400;

 

// Servo on Channel 5

const int CH5_PIN = 22;
Servo servo5;
const int SERVO5_PIN = 6; // Changed from 5

 

// Solenoid - Channel 4
const int CH4_PIN = 48 ;
const int SOLENOID_INC = 49;
const int SOLENOID_IND = 50;
const int SOLENOID_ENB = 11;

// Launch motor - Channel 1
const int CH1_PIN = 42;      // Changed from 40
const int MOTOR3_INA = 43;   // Changed from 41
const int MOTOR3_INB = 44;   // Changed from 42
const int MOTOR3_ENA = 13;    // Stays the same

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

  // Servo setup, UP and Down Servo

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

 

  // Launch motor pins

  pinMode(CH1_PIN, INPUT);

  pinMode(MOTOR3_INA, OUTPUT);

  pinMode(MOTOR3_INB, OUTPUT);

  pinMode(MOTOR3_ENA, OUTPUT);

 

  // Solenoid Pins and initialization

  pinMode(CH4_PIN, INPUT);

  pinMode(SOLENOID_INC, OUTPUT);

  pinMode(SOLENOID_IND, OUTPUT);

  pinMode(SOLENOID_ENB, OUTPUT);

  initSolenoid();

}

 

void loop() {

 

  // Servo Up/Down Logic

  unsigned long ch5Pulse = pulseIn(CH5_PIN, HIGH, 2500000);

  if (ch5Pulse > 0) {

    // Add deadband - only move if signal changes significantly

    static int lastAngle = 90;  // Remember last position

    int servoAngle = map(ch5Pulse, 1000, 2000, 20, 160);

    servoAngle = constrain(servoAngle, 0, 180);

   

    // Low Pass Filter

    if (abs(servoAngle - lastAngle) > 2) {

      servo5.write(servoAngle);

      lastAngle = servoAngle;

    }

  }

 

  // Solenoid Logic

  unsigned long ch4Pulse = pulseIn(CH4_PIN, HIGH, 2500000);

  if (ch4Pulse > FORWARD_THRESHOLD) {

    Serial.print("Solenoid")

    analogWrite(SOLENOID_ENB, 255);          // turn on

    delay(100);

    analogWrite(SOLENOID_ENB, 0);

  }  

 

  // Read left joystick (Channel 3)

  unsigned long ch3Pulse = pulseIn(CH3_PIN, HIGH, 2500000);

 

  // Read right joystick (Channel 2)

  unsigned long ch2Pulse = pulseIn(CH2_PIN, HIGH, 2500000);

 

  // Read Channel 6 for launch motor

  unsigned long ch1Pulse = pulseIn(CH1_PIN, HIGH, 2500000);

 

  // Control launch motor

  if (ch1Pulse > 0) {

    controlMotor(ch1Pulse, MOTOR3_INA, MOTOR3_INB, MOTOR3_ENA, "LAUNCH_MOTOR");

  }  

 

  // // Control left motor

  if (ch3Pulse > 0) {

    controlMotor(ch3Pulse, LEFT_INA, LEFT_INB, LEFT_ENA, "LEFT");

  }

 

  // Control right motor

  if (ch2Pulse > 0) {

    controlMotor(ch2Pulse, RIGHT_INC, RIGHT_IND, RIGHT_ENB, "RIGHT");

  }  

  Serial.println();

  delay(20);

}

 

void initSolenoid() {

  digitalWrite(SOLENOID_INC, HIGH);

  digitalWrite(SOLENOID_IND, LOW);

  analogWrite(SOLENOID_ENB, 0);   // start off

}

 

void controlMotor(unsigned long pulseWidth, int pinA, int pinB, int pinEnable, String motorName) {

  int speed = 0;

  bool forward = true;

 

  if (pulseWidth > FORWARD_THRESHOLD) {  // Stick UP - Forward

    speed = map(pulseWidth, FORWARD_THRESHOLD, 2000, 0, 255);

    forward = true;

  } else if (pulseWidth < REVERSE_THRESHOLD) {  // Stick DOWN - Reverse

    speed = map(pulseWidth, REVERSE_THRESHOLD, 900, 0, 255);

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

  Serial.print(forward ? "FWD " : "REV ");

}
