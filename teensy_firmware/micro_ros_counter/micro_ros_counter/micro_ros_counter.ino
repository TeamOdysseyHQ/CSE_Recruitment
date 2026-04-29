#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/int32.h>

rcl_publisher_t publisher;
rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;

std_msgs__msg__Int32 msg;

int counter = 0;

void setup() {

  Serial.begin(115200);
  delay(2000);

  set_microros_serial_transports(Serial);

  allocator = rcl_get_default_allocator();

  rclc_support_init(&support, 0, NULL, &allocator);

  rclc_node_init_default(
      &node,
      "teensy_counter_node",
      "",
      &support);

  rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "micro_ros_counter");

  msg.data = 0;
}

void loop() {

  msg.data = counter;

  rcl_publish(&publisher, &msg, NULL);

  counter++;

  delay(1000);
}