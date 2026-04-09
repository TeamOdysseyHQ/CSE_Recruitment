#include<Wire.h>
#include<Adafruit_BNO055.h>
#include<Adafruit_Sensor.h>
#include<utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055();
sensors_event_t event;
void setup(){
    Serial.begin(115200);
    Wire.begin();
    
    if (!bno.begin()) {
    Serial.println("BNO055 not detected!");
    while (1);
    }
    delay(1000);

    bno.setMode(OPERATION_MODE_NDOF);
    bno.setExtCrystalUse(true);
    Serial.println("BNO055 initialized successfully");

}


void loop(){
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_EULER);
    float yaw  = event.orientation.x;
    float roll = event.orientation.y;
    float pitch  = event.orientation.z;
    Serial.print("Roll :");
    Serial.print(roll,2);
    Serial.println("°");

    Serial.print("Pitch: ");
    Serial.print(pitch,2);
    Serial.println("°");

    Serial.print("yaw: ");
    Serial.print(yaw,2);
    Serial.println("°");

    Serial.println(); 
    delay(500);


}