#include <Servo.h>

Servo barrierServo;

// ----- PIN DEFINITIONS -----
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

// ----- DISTANCE SETTINGS -----
const int triggerDistance = 10; // cm

// ----- SERVO POSITIONS -----
const int CLOSED_POS = 90;
const int OPEN_POS   = 135;  // reversed direction

// ----- TIMING -----
const int closeDelayTime = 2000; // ms (2 seconds)

long duration;
int distance;
unsigned long lastDetectedTime = 0;
bool barrierOpen = false;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  barrierServo.attach(servoPin);
  barrierServo.write(CLOSED_POS);
}

void loop() {

  // ----- ULTRASONIC PULSE -----
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // ----- READ DISTANCE -----
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ----- DETECTION LOGIC -----
  if (distance > 0 && distance <= triggerDistance) {
    lastDetectedTime = millis();

    if (!barrierOpen) {
      openBarrierSmooth();
      barrierOpen = true;
    }
  }

  // ----- DELAYED CLOSE -----
  if (barrierOpen && millis() - lastDetectedTime > closeDelayTime) {
    closeBarrierSmooth();
    barrierOpen = false;
  }

  delay(50);
}

// ----- SMOOTH MOVEMENT FUNCTIONS -----

void openBarrierSmooth() {
  for (int pos = CLOSED_POS; pos <= OPEN_POS; pos++) {
    barrierServo.write(pos);
    delay(10);
  }
}

void closeBarrierSmooth() {
  for (int pos = OPEN_POS; pos >= CLOSED_POS; pos--) {
    barrierServo.write(pos);
    delay(10);
  }
}