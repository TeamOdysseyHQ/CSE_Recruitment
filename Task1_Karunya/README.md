# Task 1 — GPS Reading and NMEA Parsing

## Objective

The objective of this task is to read GPS data and extract useful information such as latitude, longitude, altitude, and timestamp by parsing NMEA sentences.



## Understanding GPS Data

GPS modules output data in a standard format called **NMEA sentences**.

One of the most important sentence types is:
$GPGGA — Global Positioning System Fix Data

Example:
$GPGGA,123519,1301.234,N,08012.345,E,1,08,0.9,15.34,M

This contains:

* Time → 123519 (HHMMSS)
* Latitude → 1301.234 (DDMM.MMMM)
* Longitude → 08012.345 (DDDMM.MMMM)
* Altitude → 15.34 meters



## Approach

Since I did not have access to the hardware (Teensy 4.1 and GPS module), I simulated GPS input using a sample NMEA string.

This allowed me to focus on parsing logic and data extraction.



## Implementation Steps

1. **Simulated GPS Input**

   * Used a hardcoded NMEA sentence for testing.

2. **Sentence Identification**

   * Checked if the input starts with `$GPGGA`.

3. **Data Splitting**

   * Split the sentence using commas.
   * Stored values in an array.

4. **Data Extraction**

   * Extracted required fields:

     * Time → index 1
     * Latitude → index 2
     * Longitude → index 4
     * Altitude → index 9

5. **Coordinate Conversion**

   * GPS provides coordinates in degrees and minutes format.
   * Converted to decimal using:

     decimal = degrees + (minutes / 60)

6. **Time Formatting**

   * Converted time from HHMMSS to HH:MM:SS format.

7. **Output Formatting**

   * Printed clean and readable output using Serial Monitor.



## Output Example

Processed GPS Data:

Time: 12:35:19
Lat: 13.020567
Lon: 80.205750
Altitude: 15.34 m



## Tools Used

* Arduino IDE (for writing and compiling code)
* C++ (Arduino framework)



## Limitations

* GPS input is simulated due to lack of hardware
* Real-time data handling is not implemented



## Future Improvements

* Integrate real GPS module using Serial communication
* Use libraries like TinyGPS++ for robust parsing
* Add error handling for invalid or incomplete data
* Extend support for multiple NMEA sentence types



## Conclusion

This project successfully demonstrates how GPS data can be parsed and converted into meaningful information. The implemented logic can be directly extended to real hardware systems.



## References

* NMEA Sentence Format: https://www.gpsinformation.org/dale/nmea.htm
* TinyGPS++ Library: https://github.com/mikalhart/TinyGPSPlus
* Arduino Documentation: https://www.arduino.cc/reference/en/
