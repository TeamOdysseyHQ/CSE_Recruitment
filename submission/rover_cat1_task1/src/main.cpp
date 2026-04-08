#include <Arduino.h>
#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/nav_sat_fix.h>
#include <TinyGPSPlus.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){while(1){};}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

rcl_publisher_t publisher;
sensor_msgs__msg__NavSatFix msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
TinyGPSPlus gps;

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  set_microros_transports();
  Serial1.begin(GPSBaud);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "/gps/fix", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, NavSatFix),
    "/gps/fix"));
}

void loop() {
  while (Serial1.available() > 0) {
    // if (gps.encode(Serial1.read())) {
      // Serial.print("Lat: ");
      // Serial.print(gps.location.lat(), 6);
      // Serial.print("Lon: ");
      // Serial.print(gps.location.lng(), 6);
      // Serial.print("Alt: ");
      // Serial.print(gps.altitude.meters(), 6);
      // Serial.print("Time: ");
      // Serial.print(gps.time.value(), 6);

      gps.encode(Serial1.read());
    }

    if (gps.location.isUpdated()) {
      msg.latitude = gps.location.lat();
      msg.longitude = gps.location.lng();
      msg.altitude = gps.altitude.meters();

      rcl_publish(&publisher, &msg, NULL);
    }
}
