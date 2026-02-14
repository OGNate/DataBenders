// Left track - Channel 3 (left joystick up/down)
const int CH3_PIN = 2;
const int LEFT_INA = 5;
const int LEFT_INB = 6;
const int LEFT_ENA = 10;

// Right track - Channel 2 (right joystick up/down)
const int CH2_PIN = 3;
const int RIGHT_INC = 7;
const int RIGHT_IND = 8;
const int RIGHT_ENB = 11;

void setup() {
  Serial.begin(9600);
  
  // Left motor pins
  pinMode(CH3_PIN, INPUT);
  pinMode(LEFT_INA, OUTPUT);
  pinMode(LEFT_INB, OUTPUT);
  pinMode(LEFT_ENA, OUTPUT);
  
  // Right motor pins
  pinMode(CH2_PIN, INPUT);
  pinMode(RIGHT_INC, OUTPUT);
  pinMode(RIGHT_IND, OUTPUT);
  pinMode(RIGHT_ENB, OUTPUT);
}

void loop() {
  // Read left joystick (Channel 3)
  unsigned long ch3Pulse = pulseIn(CH3_PIN, HIGH, 25000);
  
  // Read right joystick (Channel 2)
  unsigned long ch2Pulse = pulseIn(CH2_PIN, HIGH, 25000);
  
  // Control left motor
  if (ch3Pulse > 0) {
    controlMotor(ch3Pulse, LEFT_INA, LEFT_INB, LEFT_ENA, "LEFT");
  }
  
  // Control right motor
  if (ch2Pulse > 0) {
    controlMotor(ch2Pulse, RIGHT_INC, RIGHT_IND, RIGHT_ENB, "RIGHT");
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