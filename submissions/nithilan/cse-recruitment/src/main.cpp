#include <Arduino.h>
#include <lwgps/lwgps.h>

#ifndef USE_MICROROS
#define USE_MICROROS 0
#endif

#ifndef PRINT_RAW_GPS
#define PRINT_RAW_GPS 0
#endif

// micro-ROS serial transport uses the same USB serial stream as SerialMonitor.
// Disable raw NMEA text output in this mode to avoid binary/text interleaving.
#if USE_MICROROS
#undef PRINT_RAW_GPS
#define PRINT_RAW_GPS 0
#endif

#if USE_MICROROS
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>

#include <sensor_msgs/msg/nav_sat_fix.h>
#include <sensor_msgs/msg/nav_sat_status.h>
#endif

#define ARDUINO_GPS_RX 0
#define ARDUINO_GPS_TX 1

// The GPS module L86 GNSS defaults to 9600 baud
#define GPS_BAUD_RATE 9600

// Debug + rate controls
#define GPS_POLL_INTERVAL_MS 50      // read serial every 50ms
#define GPS_OUTPUT_INTERVAL_MS 1000  // print/publish once per second

// Use hardware UART on pins 0/1 for GPS.
#define gpsPort Serial1

static lwgps_t gps;

// This is the hardware serial port on pins 0/1.
#define SerialMonitor Serial

static uint32_t last_poll_ms = 0;
static uint32_t last_output_ms = 0;

#if PRINT_RAW_GPS
static char nmea_line[128];
static size_t nmea_idx = 0;
#endif

#if USE_MICROROS
rcl_publisher_t gps_publisher;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_support_t support;
sensor_msgs__msg__NavSatFix gps_fix_msg;

#define LED_PIN 13
#define RCCHECK(fn)                    \
    {                                  \
        rcl_ret_t rc = fn;             \
        if (rc != RCL_RET_OK)          \
        {                              \
            while (1)                  \
            {                          \
                digitalWrite(LED_PIN, !digitalRead(LED_PIN)); \
                delay(100);            \
            }                          \
        }                              \
    }
#define RCSOFTCHECK(fn)                \
    {                                  \
        rcl_ret_t rc = fn;             \
        if (rc != RCL_RET_OK)          \
        {                              \
        }                              \
    }

static void publish_gps_fix()
{
    const uint32_t ms = millis();

    gps_fix_msg.header.stamp.sec = static_cast<int32_t>(ms / 1000UL);
    gps_fix_msg.header.stamp.nanosec = (ms % 1000UL) * 1000000UL;
    gps_fix_msg.latitude = gps.latitude;
    gps_fix_msg.longitude = gps.longitude;
    gps_fix_msg.altitude = gps.altitude;
    gps_fix_msg.status.service = sensor_msgs__msg__NavSatStatus__SERVICE_GPS;
    gps_fix_msg.status.status = gps.fix
                                    ? static_cast<int8_t>(sensor_msgs__msg__NavSatStatus__STATUS_FIX)
                                    : static_cast<int8_t>(sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX);
    gps_fix_msg.position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN;

    RCSOFTCHECK(rcl_publish(&gps_publisher, &gps_fix_msg, NULL));
}
#endif

void setup()
{
    gpsPort.begin(GPS_BAUD_RATE);
    SerialMonitor.begin(9600);
    lwgps_init(&gps);

#if USE_MICROROS
    set_microros_serial_transports(Serial);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(2000);

    allocator = rcl_get_default_allocator();

    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    RCCHECK(rclc_node_init_default(&node, "teensy_gps_node", "", &support));
    RCCHECK(rclc_publisher_init_default(
        &gps_publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, NavSatFix),
        "/gps/fix"));

    sensor_msgs__msg__NavSatFix__init(&gps_fix_msg);
#endif
}

void loop()
{
    const uint32_t now = millis();

    // Reduced polling rate
    if (now - last_poll_ms < GPS_POLL_INTERVAL_MS)
    {
        return;
    }
    last_poll_ms = now;

    while (gpsPort.available())
    {
        char c = static_cast<char>(gpsPort.read());

#if PRINT_RAW_GPS
        // Collect and print raw NMEA lines
        if (c == '$')
        {
            nmea_idx = 0;
        }

        if (nmea_idx < sizeof(nmea_line) - 1)
        {
            nmea_line[nmea_idx++] = c;
        }

        if (c == '\n' && nmea_idx > 0)
        {
            nmea_line[nmea_idx] = '\0';
            SerialMonitor.print("RAW: ");
            SerialMonitor.print(nmea_line);
            nmea_idx = 0;
        }
#endif

        if (lwgps_process(&gps, &c, 1))
        {
            // Throttle parsed output/publish rate
            if (millis() - last_output_ms < GPS_OUTPUT_INTERVAL_MS)
            {
                continue;
            }
            last_output_ms = millis();

#if USE_MICROROS
            if (gps.is_valid)
            {
                publish_gps_fix();
            }
#else
            SerialMonitor.print("Latitude: ");
            SerialMonitor.print(gps.latitude, 6);
            SerialMonitor.println(" degrees");

            SerialMonitor.print("Longitude: ");
            SerialMonitor.print(gps.longitude, 6);
            SerialMonitor.println(" degrees");

            SerialMonitor.print("Altitude: ");
            SerialMonitor.print(gps.altitude, 2);
            SerialMonitor.println(" meters");
            SerialMonitor.println();
#endif
        }
    }
}