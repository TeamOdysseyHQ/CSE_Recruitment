
#include <Arduino.h>
#define ENCODER_PIN_A  2   
#define ENCODER_PIN_B  3   


const int   PPR                = 7;                          
const float WHEEL_DIAMETER_M   = 0.1050;                     
const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER_M;     
const float DIST_PER_PULSE     = WHEEL_CIRCUMFERENCE / PPR;  


const unsigned long INTERVAL_MS = 1000;  // Calculate RPM every 1 second


volatile long  pulseCount       = 0;   // Total pulses (all time)
volatile long  pulsesInInterval = 0;   // Pulses in current interval
volatile bool  direction        = true; // true = Forward, false = Backward


unsigned long lastTime   = 0;
float         rpm        = 0.0;
float         distanceM  = 0.0;


void encoderISR() {
  direction = (digitalRead(ENCODER_PIN_B) == HIGH);

  if (direction) {
    pulseCount++;    // Moving forward
  } else {
    pulseCount--;    // Moving backward
  }

  pulsesInInterval++;  // Always count magnitude for RPM
}


void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);  

  // Configure encoder pins with pull-up resistors
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // Attach interrupt to Channel A — triggers on RISING edge
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, RISING);

  lastTime = millis();

 
}


void loop() {
  unsigned long currentTime = millis();


  if (currentTime - lastTime >= INTERVAL_MS) {

 
    noInterrupts();
    long intervalPulses = pulsesInInterval;
    long totalPulses    = pulseCount;
    bool currentDir     = direction;
    pulsesInInterval    = 0;   // Reset for next interval
    interrupts();

    // RPM Calculation:
    float revolutions = (float)intervalPulses / PPR;
    rpm = revolutions * 60.0;

    // Distance Calculation:
    distanceM = abs(totalPulses) * DIST_PER_PULSE;

   
    String dirStr = currentDir ? "Forward" : "Backward";

    Serial.println("----------------------------------------");
    Serial.print("RPM       : ");
    Serial.println(rpm, 1);

    Serial.print("Distance  : ");
    Serial.print(distanceM, 3);
    Serial.println(" meters");

    Serial.print("Pulses    : ");
    Serial.println(abs(totalPulses));

    Serial.print("Direction : ");
    Serial.println(dirStr);
    Serial.println("----------------------------------------\n");

    lastTime = currentTime;
  }
}