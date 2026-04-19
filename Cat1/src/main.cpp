#include <Arduino.h>
#define enc_a 2
#define enc_b 3
const int ppr=28;
volatile float wheelrad;
const float pi=3.1415;
unsigned long tp = 0, tc=0;
bool measdone = 0;
float rpm = 0;
volatile long pulcount=0;
volatile bool direction=1;
float dist=0;
float rot=0;

void encisr(void)
{
    if (digitalRead(enc_a)){
        pulcount++;
        direction=1;
    }
    else{
        pulcount--;
        direction=-1;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Enter the radius of the wheel (in cms)");
    wheelrad=Serial.parseFloat();
    pinMode(enc_a,INPUT);
    pinMode(enc_b,INPUT);
}

void loop()
{
    tc=millis();
    if (tc-tp>=1000){ //duration is 1 second
        detachInterrupt(enc_a);
        tp=tc;
        rot=pulcount/ppr;
        rpm=(rot)*60000;
        dist=2*pi*(float(wheelrad)/100)*rot;
    }
    attachInterrupt(digitalPinToInterrupt(enc_a), encisr, RISING);
    Serial.print("RPM: ");
    Serial.println(rpm);
    Serial.print("Distance Travelled (in m): ");
    Serial.println(dist);
    Serial.print("Pulses: ");
    Serial.println(pulcount);
    Serial.print("Direction");
    Serial.println(direction==1 ? "Forward" : "Backward");
};