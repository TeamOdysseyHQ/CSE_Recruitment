#include <Arduino.h>

#define ENCODER_A 2
#define ENCODER_B 3

volatile long pulseCount = 0;
volatile int direction = 1; // 1 = forward, -1 = backward

const int PPR = 7;
const float WHEEL_RADIUS = 0.05; // meters - change as per given

unsigned long lastTime = 0;

float rpm = 0;
float distance = 0;

void handleEncoder() {
    int b = digitalRead(ENCODER_B);

    if (b == HIGH) {
        direction = 1;
        pulseCount++;
    } else {
        direction = -1;
        pulseCount--;
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A), handleEncoder, RISING);

    lastTime = millis();
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastTime >= 1000) {

        // Safe read from interrupt
        noInterrupts();
        long pulses = pulseCount;
        pulseCount = 0;
        interrupts();

        // RPM calculation
        rpm = (abs(pulses) * 60.0) / PPR;

        // Distance calculation
        float rotations = abs(pulses) / (float)PPR;
        float circumference = 2 * PI * WHEEL_RADIUS;
        distance += rotations * circumference;

        // Output
        Serial.print("RPM: ");
        Serial.print(rpm);

        Serial.print(" | Distance: ");
        Serial.print(distance);

        Serial.print(" | Pulses: ");
        Serial.print(pulses);

        Serial.print(" | Direction: ");
        if (direction == 1)
            Serial.println("Forward");
        else
            Serial.println("Backward");

        lastTime = currentTime;
    }
}