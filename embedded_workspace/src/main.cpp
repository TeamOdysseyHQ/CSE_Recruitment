#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// Sensor Libraries
#include <TinyGPSPlus.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// ROS 2 Libraries
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// ROS 2 Message Types
#include <std_msgs/msg/float32.h>
#include <sensor_msgs/msg/nav_sat_fix.h>
#include <sensor_msgs/msg/imu.h>

// ---------------------------------------------------------
// COMPONENT DECLARATIONS
// ---------------------------------------------------------
// 1. GPS
TinyGPSPlus gps;
#define SerialGPS Serial1 
static const uint32_t GPSBaud = 9600; // Common for L86

// 2. IMU
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

// 3. ENCODER
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
const float PPR = 7.0; 
const float GEAR_RATIO = 1.0; 
const float WHEEL_CIRCUMFERENCE_METERS = 0.314;

volatile long pulse_count = 0;
volatile int current_direction = 1;
unsigned long last_time = 0;
long last_pulse_count = 0;
float current_rpm = 0.0;

// ---------------------------------------------------------
// MICRO-ROS DEFINITIONS
// ---------------------------------------------------------
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t executor;

// Publishers
rcl_publisher_t rpm_publisher;
rcl_publisher_t gps_publisher;
rcl_publisher_t imu_publisher;

// Messages
std_msgs__msg__Float32 rpm_msg;
sensor_msgs__msg__NavSatFix gps_msg;
sensor_msgs__msg__Imu imu_msg;

// Timers
rcl_timer_t pub_timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  while(1){
    delay(100);
  }
}

// ---------------------------------------------------------
// INTERRUPT SERVICE ROUTINE FOR ENCODER
// ---------------------------------------------------------
void encoder_isr() {
  int a = digitalReadFast(ENCODER_PIN_A);
  int b = digitalReadFast(ENCODER_PIN_B);
  if (a == b) { pulse_count++; current_direction = 1; } 
  else { pulse_count--; current_direction = -1; }
}

// ---------------------------------------------------------
// SENSOR DATA PUBLISHING CALLBACK
// ---------------------------------------------------------
void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{  
  (void) last_call_time;
  if (timer != NULL) {

    // --- 1. ENCODER RPM ---
    rpm_msg.data = current_rpm;
    RCSOFTCHECK(rcl_publish(&rpm_publisher, &rpm_msg, NULL));

    // --- 2. IMU DATA ---
    imu::Quaternion quat = bno.getQuat();
    imu_msg.orientation.w = quat.w();
    imu_msg.orientation.x = quat.x();
    imu_msg.orientation.y = quat.y();
    imu_msg.orientation.z = quat.z();
    
    // Assign generic covariance to validate the type format properly downstream
    for(size_t i = 0; i < 9; i++) imu_msg.orientation_covariance[i] = 0.0;
    
    RCSOFTCHECK(rcl_publish(&imu_publisher, &imu_msg, NULL));

    // --- 3. GPS DATA ---
    if (gps.location.isValid()) {
      gps_msg.latitude = gps.location.lat();
      gps_msg.longitude = gps.location.lng();
      gps_msg.altitude = gps.altitude.meters();
    } else {
      gps_msg.latitude = 0.0; // Wait for satellite fix
      gps_msg.longitude = 0.0;
      gps_msg.altitude = 0.0;
    }
    RCSOFTCHECK(rcl_publish(&gps_publisher, &gps_msg, NULL));
  }
}

// ---------------------------------------------------------
// SETUP
// ---------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Initialize GPS Serial
  SerialGPS.begin(GPSBaud);

  // Initialize IMU
  if (!bno.begin()) {
    // Force fail if no IMU is wired
  }

  // Initialize Encoder Pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoder_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoder_isr, CHANGE);

  // micro-ROS Boot up sequence
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();

  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "teensy_universal_node", "", &support));

  // Initialize all 3 Publishers mapped to std_msgs and sensor_msgs!
  RCCHECK(rclc_publisher_init_default(
    &rpm_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "/motor/rpm"));

  RCCHECK(rclc_publisher_init_default(
    &gps_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, NavSatFix),
    "/gps/fix"));

  RCCHECK(rclc_publisher_init_default(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
    "/imu/data"));

  // Unified publishing timer (every 100ms)
  RCCHECK(rclc_timer_init_default(&pub_timer, &support, RCL_MS_TO_NS(100), timer_callback));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &pub_timer));
}

// ---------------------------------------------------------
// MAIN LOOP
// ---------------------------------------------------------
void loop() {
  unsigned long current_time = millis();

  // Parse GPS
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Calculate RPM strictly outside interrupts natively every 1000ms (1 second)
  if (current_time - last_time >= 1000) {
    noInterrupts();
    long current_pulses = pulse_count;
    interrupts();

    long pulses_delta = current_pulses - last_pulse_count;
    last_pulse_count = current_pulses;

    float delta_time_minutes = (current_time - last_time) / 60000.0;
    float shaft_revolutions = ((float)pulses_delta / PPR) / GEAR_RATIO;
    current_rpm = shaft_revolutions / delta_time_minutes;

    last_time = current_time;
  }

  // Execute internal MicroROS pipelines (fires off the timer_callback above)
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
}
