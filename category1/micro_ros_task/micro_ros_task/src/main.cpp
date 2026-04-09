#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/float32.h>

#define ENCODER_A 2
#define ENCODER_B 3

volatile long pulseCount = 0;
volatile int direction = 1;

const int PPR = 7;
const float WHEEL_RADIUS = 0.05;

unsigned long lastTime = 0;

float rpm = 0;
float distance = 0;

// micro-ROS variables
rcl_publisher_t publisher;
rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;

std_msgs__msg__Float32 msg;

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

    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A), handleEncoder, RISING);

    // micro-ROS transport
    set_microros_transports();

    delay(2000); // allow agent to connect

    allocator = rcl_get_default_allocator();

    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, "encoder_node", "", &support);

    rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "/motor/rpm"
    );

    lastTime = millis();
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastTime >= 1000) {
        long pulses = pulseCount;

        rpm = (abs(pulses) * 60.0) / PPR;

        float rotations = pulses / (float)PPR;
        float circumference = 2 * PI * WHEEL_RADIUS;
        distance += rotations * circumference;

        // Publish RPM
        msg.data = rpm;
        rcl_publish(&publisher, &msg, NULL);

        // Debug output
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

        pulseCount = 0;
        lastTime = currentTime;
    }
}