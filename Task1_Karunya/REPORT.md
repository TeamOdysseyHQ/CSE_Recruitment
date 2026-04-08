# Task 1 Report — GPS Data Parsing

## Step 1: Understanding the Problem

The task required reading GPS data and extracting useful parameters such as latitude, longitude, altitude, and timestamp.

Initially, I did not know how GPS modules output data, so I started by researching GPS data formats.



## Step 2: Research on GPS Data

Through research, I learned that GPS modules output data in the form of **NMEA sentences**.

One important sentence type is `$GPGGA`, which contains essential information like:

* Time
* Latitude
* Longitude
* Altitude

Example:
$GPGGA,123519,1301.234,N,08012.345,E,1,08,0.9,15.34,M

From this, I understood that:

* Data is comma-separated
* Each value has a fixed position (index)



## Step 3: Initial Approach

My initial idea was to directly read GPS data from hardware using Serial communication.

However, since I did not have access to the GPS module or Teensy board, I decided to simulate the GPS input using a hardcoded NMEA sentence.

This allowed me to focus on solving the parsing problem.



## Step 4: Identifying Required Data

From the `$GPGGA` sentence, I identified the required indices:

* Time → index 1
* Latitude → index 2
* Latitude direction → index 3
* Longitude → index 4
* Longitude direction → index 5
* Altitude → index 9

This mapping was obtained by analyzing the NMEA format.



## Step 5: Parsing Strategy

To extract the data, I implemented the following logic:

1. Check if the string starts with `$GPGGA`
2. Traverse the string character by character
3. Split the string whenever a comma is encountered
4. Store the split values into an array

Initially, I considered using external libraries, but I chose manual parsing to better understand the data structure.



## Step 6: Coordinate Conversion

The latitude and longitude values were not directly usable because they were in **degrees and minutes format**.

Example:
1301.234 → 13 degrees and 1.234 minutes

My first assumption was that the number could be directly used, but after research, I realized it needed conversion.

So I used the formula:
decimal = degrees + (minutes / 60)

This converts the value into standard decimal format.



## Step 7: Time Formatting

The timestamp was provided in HHMMSS format.

Example:
123519 → 12 hours, 35 minutes, 19 seconds

To improve readability, I converted it into:
12:35:19



## Step 8: Output Formatting

Initially, I printed raw values, but it was difficult to read.

So I improved the output by formatting it clearly:

* Added labels (Lat, Lon, Time, Altitude)
* Printed values with proper precision



## Step 9: Challenges Faced

* Understanding NMEA sentence structure
* Identifying correct indices for required data
* Converting coordinates correctly
* Handling string manipulation in Arduino C++



## Step 10: Improvements Made

* Added timestamp formatting
* Improved output readability
* Structured the code for clarity
* Used conditional checks for correctness



## Step 11: Use of Resources

I used the following resources during the task:

* Online documentation for NMEA sentence format
* Arduino documentation for string handling
* ChatGPT for understanding parsing logic and debugging code



## Step 12: Conclusion

This task helped me understand how GPS data is structured and how it can be processed programmatically.

The implemented solution successfully extracts and converts GPS data into meaningful values.

The same logic can be extended to real hardware by replacing the simulated input with actual serial data from a GPS module.
