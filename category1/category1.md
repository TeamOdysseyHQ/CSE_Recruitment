Created to files main.cpp and platformio.ini

headerfiles in main.cpp-
#include <Wire.h>-Required to initialize and use I2C on Teensy

#include <Adafruit_BNO055.h>-Main library to interact with the sensor
Handles initialization + reading orientation
#include <Adafruit_Sensor.h>-Required dependency for Adafruit sensor libraries
Provides standardized sensor data structures
#include <utility/imumaths.h>-Used for vector, quaternion, and math operations

In the code-
bno.setExtCrystalUse(true)- improves accuracy 


platformio.ini
Board setup-
platform = teensy
board = teensy41
framework = arduino

Libraries-
lib_deps =
    adafruit/Adafruit BNO055
    adafruit/Adafruit Unified Sensor

This is for printing reading and printing IMU data 


ROS2 integration - thought of doing it later and submitting this for now 

reference -https://chatgpt.com/share/69d73b87-cb84-8322-9faa-d1a1eded7fd4