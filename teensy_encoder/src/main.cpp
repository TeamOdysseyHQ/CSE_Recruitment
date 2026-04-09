#include <Arduino.h>


const int PIN_ENCODER_A = 2;
const int PIN_ENCODER_B = 3;

const float COUNTS_PER_REV = 28.0;

const float WHEEL_DIAMETER_M = 0.065;
const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER_M;

const unsigned long CALC_INTERVAL_MS = 500;

volatile long pulseCount = 0;
volatile int currentDirection = 1; 
volatile bool lastA = false;
volatile bool lastB = false;

unsigned long lastTime = 0;
long lastPulseCount = 0;


void isrChannelA() {
  bool stateA = digitalRead(PIN_ENCODER_A);
  bool stateB = digitalRead(PIN_ENCODER_B);
  if (stateA != lastA) {
    if (stateA == stateB) { currentDirection = 1;  pulseCount++; }
    else                  { currentDirection = -1; pulseCount--; }
    lastA = stateA;
  }
}

void isrChannelB() {
  bool stateA = digitalRead(PIN_ENCODER_A);
  bool stateB = digitalRead(PIN_ENCODER_B);
  if (stateB != lastB) {
    if (stateA != stateB) { currentDirection = 1;  pulseCount++; }
    else                  { currentDirection = -1; pulseCount--; }
    lastB = stateB;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);

  pinMode(PIN_ENCODER_A, INPUT_PULLUP);
  pinMode(PIN_ENCODER_B, INPUT_PULLUP);

  lastA = digitalRead(PIN_ENCODER_A);
  lastB = digitalRead(PIN_ENCODER_B);

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), isrChannelA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), isrChannelB, CHANGE);

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastTime;

  if (deltaTime >= CALC_INTERVAL_MS) {
 
    noInterrupts();
    long currentPulses = pulseCount;
    int dir = currentDirection;
    interrupts();

    long pulsesThisInterval = currentPulses - lastPulseCount;

    float rpm = (abs(pulsesThisInterval) / COUNTS_PER_REV) * (60000.0 / deltaTime);

    float distance = (abs(currentPulses) / COUNTS_PER_REV) * WHEEL_CIRCUMFERENCE;

    String dirStr = (dir == 1) ? "Forward" : "Backward";

    Serial.print("RPM: ");
    Serial.println(rpm);

    Serial.print("Distance: ");
    Serial.print(distance, 4);
    Serial.println(" meters");

    Serial.print("Pulses (Interval): ");
    Serial.println(abs(pulsesThisInterval));

    Serial.print("Pulses (Total): ");
    Serial.println(abs(currentPulses));

    Serial.print("Direction: ");
    Serial.println(dirStr);

    Serial.println("--------------------");

    lastPulseCount = currentPulses;
    lastTime = currentTime;
  }
}