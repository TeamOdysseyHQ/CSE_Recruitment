# Category 3 – Embedded Systems & MicroROS

## My Understanding of Task 3 and Advance Task

1. It aims to read sensor data using embedded programming and publishing it using ROS2 via micro-ROS.
2. The main objective is to interface hardware (encoder) with software (ROS2 topics).
3. We calculate RPM and distance travelled using encoder pulses
4. microcontroller like Teensy communicates with ROS2 with the help of microROS

Basically how rover senses the motion


# Task 3 - Encoder-Based RPM & Odometry (Interrupt-Driven)

We calculate the RPM, distance travelled, pulse count, and direction using encoder signals with interrupt-based programming

Used PlatformIO in VSCode to create a new folder where Board is Teensy4.1 and framework is Arduino.

 - According to what I understood in the code, Chaneel A trigger the interrupt and Channel B helps detect the direction of motion
 - So everytime pulse occurs Channel B is read therefore direction is identified
 
Initially the code given used IRAM_ATTR for handleEncoder() function but this caused an error. After checking its understood that IRAM-ATTR is used for ESP32 and Teensy doesnt support it.


# Advance Task

We need to publish the encoder RPM data to ROS2 topic using micro-ROS

Since the previous needs to be present, I created a separate PlatformIO project for micro-ROS to avoid dependency conflicts. Initialized micro-ROS transport for communication between Teensy and ROS2.

Also by adding Micro-ROS in the same project as encoder it caused Ethernet-related build errors. This was sovled by Creating a separate project for micro-ROS as mentioned before


# Verification
 * The implementation was verified up to the build stage using PlatformIO in VS Code.
 * he project compiled successfully without any errors
 * I attempted to upload the code, but it required a physical Teensy board and pressing the hardware program button, which was not available.

Therefore it is verified that:
 * Successful build
 * Logical verification of interrupt handling
 * Correct codes for determining RPM and distance
 * Proper initialization of micro_ROS
 
 
Link to LLM used : https://chatgpt.com/share/69d78c76-355c-8321-9016-293f1d2ab111
