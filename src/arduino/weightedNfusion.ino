#include <SoftwareSerial.h>

//
// PIN ASSIGNMENTS
//
#define TRIG 2        // Sonar Trigger
#define ECHO 3        // Sonar Echo
#define IR_PIN 4      // IR Obstacle Sensor (digital output)

SoftwareSerial lidarSerial(8, 9);  // RX=8 (LIDAR TX), TX=9 (LIDAR RX)

//
// WEIGHTED FUSION PARAMETERS
//
float w_lidar = 0.5663;   // LIDAR weight (update from MATLAB if needed)
float w_sonar = 0.4337;   // Sonar weight
 
//
// SAMPLE COUNT
//
int maxCount = 100;    // Number of fused readings you want
int count = 0;

//
// LIDAR MEMORY: last valid reading
//
int lastValidLidar = 0;   // stores last good LIDAR distance in cm


// -----------------------------
// SETUP
// -----------------------------
void setup() {
  Serial.begin(9600);
  lidarSerial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_PIN, INPUT);

  // CSV header
  Serial.println("sample,sonar,lidar,ir,fused");
}


// -----------------------------
// READ SONAR (HC-SR04)
// -----------------------------
float readSonar() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float dist = duration * 0.034 / 2.0;   // in cm
  return dist;
}


// -----------------------------
// READ LIDAR (TF-Luna, UART)
// with last-valid replacement
// -----------------------------
int readLidar() {
  if (lidarSerial.available() >= 9) {
    // Check frame header 0x59 0x59
    if (lidarSerial.read() == 0x59 && lidarSerial.read() == 0x59) {

      int low = lidarSerial.read();
      int high = lidarSerial.read();
      int distance = low + high * 256;   // cm

      // Discard the remaining 5 bytes (strength, temp, checksum)
      for (int i = 0; i < 5; i++) {
        lidarSerial.read();
      }

      // Basic validity check + range limit
      if (distance > 0 && distance < 300) {   // adjust upper bound if needed
        lastValidLidar = distance;            // update memory
        return distance;
      }
      // else fall through to "invalid" handling below
    }
  }

  // If no valid frame or out-of-range: return last good value
  return lastValidLidar;
}


// -----------------------------
// MAIN LOOP (Fusion + CSV)
// -----------------------------
void loop() {

  // Stop after desired number of samples
  if (count >= maxCount) {
    Serial.println("Finished.");
    while (1);   // halt
  }

  // Read sensors
  float sonarDist = readSonar();
  int lidarDist   = readLidar();
  int irState     = digitalRead(IR_PIN);

  float fusedDist;

  // --------- FUSION LOGIC ---------
  if (irState == LOW) {
    // IR triggered → object very close / reflective
    // Safety first: choose the closer distance
    fusedDist = min(sonarDist, (float)lidarDist);
  }
  else {
    // Normal weighted fusion using latest LIDAR value
    fusedDist = (w_lidar * lidarDist + w_sonar * sonarDist) /
                (w_lidar + w_sonar);
  }

  // --------- CSV OUTPUT ---------
  Serial.print(count);       Serial.print(",");
  Serial.print(sonarDist);   Serial.print(",");
  Serial.print(lidarDist);   Serial.print(",");
  Serial.print(irState);     Serial.print(",");
  Serial.println(fusedDist);

  count++;
  delay(200);
}
