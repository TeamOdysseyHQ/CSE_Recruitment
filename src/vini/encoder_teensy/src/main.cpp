#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float32.h>

#define ENCODER_A 2
#define ENCODER_B 3

#define PPR 7
#define WHEEL_DIAMETER 0.065

#define wheel_circumference (WHEEL_DIAMETER * 3.1416)

volatile long pulseC = 0;
volatile int direction = 1;

unsigned long lastT = 0;
long lastPcount = 0;
float rpm=0;
float distance=0;

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rcl_publisher_t publisher;
rclc_executor_t executor;
std_msgs__msg__Float32 rpm_msg;



void encoderISR() {
  if(digitalRead(ENCODER_B)== HIGH) {
    direction = 1;
    pulseC++;
  } else {
    direction = -1;
    pulseC--;
  }
}



void setup() {
 Serial.begin(115200);
set_microros_transports();
 pinMode(ENCODER_A, INPUT_PULLUP);
 pinMode(ENCODER_B, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, RISING);

 // micro-ROS setup
  allocator = rcl_get_default_allocator();
  rclc_support_init(&support, 0, NULL, &allocator);
  rclc_node_init_default(&node, "teensy_node", "", &support);
  rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/motor/rpm");
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  

}

void loop() {
  unsigned long current = millis();
  if (current - lastT >= 1000) {
    long startPcount;
    int startDirection;

    noInterrupts();
    startPcount = pulseC;
    startDirection = direction;
    interrupts();

    long diff = abs(startPcount - lastPcount);
    float hap = (current - lastT) / 1000.0;
    rpm = ((float)diff / PPR) / hap * 60.0;
    distance += ((float)diff / PPR) * wheel_circumference;

    rpm_msg.data = rpm;
    rcl_publish(&publisher, &rpm_msg, NULL);

    Serial.print("RPM: ");        Serial.println(rpm, 1);
    Serial.print("Distance: ");   Serial.print(distance, 2);
    Serial.println(" meters");
    Serial.print("Pulses: ");     Serial.println(abs(startPcount));
    Serial.print("Direction: ");  Serial.println(startDirection == 1 ? "Forward" : "Reverse");
    Serial.println();
    
    lastPcount = startPcount;
    lastT = current;

    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));

}
}

