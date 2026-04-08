void setup() {
  Serial.begin(9600);  // Start serial communication
}

void loop() {

  //  Simulated GPS NMEA sentence
  String gpsData = "$GPGGA,123519,1301.234,N,08012.345,E,1,08,0.9,15.34,M";

  //  Check if it's GPGGA sentence
  if (gpsData.startsWith("$GPGGA")) {

    // Split the data using commas
    String parts[15];
    int index = 0;
    int lastIndex = 0;

    for (int i = 0; i < gpsData.length(); i++) {
      if (gpsData[i] == ',') {
        parts[index++] = gpsData.substring(lastIndex, i);
        lastIndex = i + 1;
      }
    }
    parts[index] = gpsData.substring(lastIndex); // Last value

    // Extract raw values
    String timeRaw = parts[1];
    String latRaw = parts[2];
    String latDir = parts[3];
    String lonRaw = parts[4];
    String lonDir = parts[5];
    String altitude = parts[9];

    //  Convert time (HHMMSS → HH:MM:SS)
    String hours = timeRaw.substring(0, 2);
    String minutes = timeRaw.substring(2, 4);
    String seconds = timeRaw.substring(4, 6);
    String formattedTime = hours + ":" + minutes + ":" + seconds;

    //  Convert latitude (DDMM.MMMM → decimal)
    float lat = latRaw.substring(0, 2).toFloat() +
                latRaw.substring(2).toFloat() / 60.0;

    if (latDir == "S") lat = -lat;

    //  Convert longitude (DDDMM.MMMM → decimal)
    float lon = lonRaw.substring(0, 3).toFloat() +
                lonRaw.substring(3).toFloat() / 60.0;

    if (lonDir == "W") lon = -lon;

    //  Print final output
    Serial.println("Processed GPS Data:");

    Serial.print("Time: ");
    Serial.println(formattedTime);

    Serial.print("Lat: ");
    Serial.println(lat, 6);

    Serial.print("Lon: ");
    Serial.println(lon, 6);

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.println("----------------------");
  }

  delay(3000);  // Repeat every 3 seconds
}