#include <SoftwareSerial.h>

// SoftwareSerial(RX, TX)
SoftwareSerial lidarSerial(8, 9); // RX=8, TX=9

int count = 0;
int maxCount = 100;

// Calibration offset for LiDAR (from your Excel analysis)
// d_cal = d_raw - offset
const float LIDAR_OFFSET = 1.0;   // cm

void setup() {
  Serial.begin(115200);        // For printing to PC
  lidarSerial.begin(115200);   // For TF-Luna

  // ---- CSV header (optional but recommended) ----
  // Format: sample,raw_cm,cal_cm
  Serial.println("sample,raw,cal");
}

void loop() {

  // Stop after N samples
  if (count >= maxCount) {
    while (1);   // freeze program, no extra prints to keep CSV clean
  }

  // Check if at least 9 bytes are available
  if (lidarSerial.available() >= 9) {

    // Look for frame header 0x59 0x59
    if (lidarSerial.read() == 0x59) {
      if (lidarSerial.read() == 0x59) {

        int lowByte  = lidarSerial.read();
        int highByte = lidarSerial.read();

        // Raw distance from TF-Luna (cm)
        float distance_raw = lowByte + highByte * 256;

        // Skip the remaining 5 bytes (strength, temp, checksum, etc.)
        for (int i = 0; i < 5; i++) {
          lidarSerial.read();
        }

        // Apply calibration offset
        float distance_cal = distance_raw - LIDAR_OFFSET;

        // ---- CSV-style output: sample,raw_cm,cal_cm ----
        Serial.print(count);
        Serial.print(",");
        Serial.print(distance_raw, 3);   // 3 decimal places
        Serial.print(" ");
        Serial.println(distance_cal, 3); // newline ends the row

        count++;
        delay(500);
      }
    }
  }
}
