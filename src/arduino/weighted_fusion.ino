#include <SoftwareSerial.h>

// ------------------------------------
// PIN ASSIGNMENTS
// ------------------------------------
#define TRIG 2
#define ECHO 3
#define IR_PIN 4

SoftwareSerial lidarSerial(8, 9);  // RX=8, TX=9

// Fusion weights
float w_lidar = 0.3;
float w_sonar = 0.7;

// Sample variables
int count = 0;
int maxCount = 50;    // User will choose this value
bool sampleCountChosen = false;

// ------------------------------------
// SETUP
// ------------------------------------
void setup() {
  Serial.begin(9600);
  lidarSerial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_PIN, INPUT);

  Serial.println("Enter number of samples to collect (e.g., 50):");
}

// ------------------------------------
// READ SONAR
// ------------------------------------
float readSonar() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;   // cm
}

// ------------------------------------
// READ LIDAR
// ------------------------------------
int readLidar() {
  if (lidarSerial.available() >= 9) {
    if (lidarSerial.read() == 0x59 && lidarSerial.read() == 0x59) {

      int low = lidarSerial.read();
      int high = lidarSerial.read();
      int distance = low + high * 256;

      for (int i = 0; i < 5; i++) lidarSerial.read();

      return distance;
    }
  }
  return -1;
}

// ------------------------------------
// MAIN LOOP
// ------------------------------------
void loop() {

  // First wait for user input
  if (!sampleCountChosen) {
    if (Serial.available()) {
      maxCount = Serial.parseInt();
      if (maxCount > 0) {
        sampleCountChosen = true;
        Serial.print("Collecting ");
        Serial.print(maxCount);
        Serial.println(" samples...");
        Serial.println("sample,sonar,lidar,ir,fused");
      }
    }
    return;
  }

  // Stop when reached max samples
  if (count >= maxCount) {
    Serial.println("Finished.");
    while (1);
  }

  // ---- Read sensors ----
  float sonarDist = readSonar();
  int lidarDist = readLidar();
  int irState = digitalRead(IR_PIN);

  float fusedDist;

  // ---- WEIGHTED FUSION ----
  if (irState == LOW) {
    fusedDist = min(sonarDist, (float)lidarDist);
  }
  else if (lidarDist > 0) {
    fusedDist = (w_lidar * lidarDist + w_sonar * sonarDist) /
                (w_lidar + w_sonar);
  }
  else {
    fusedDist = sonarDist;
  }

  // ---- CSV OUTPUT ----
  //Serial.print(count); Serial.print(",");
  Serial.print(sonarDist); Serial.print(", ");
  Serial.print(lidarDist); Serial.print(", ");
  //Serial.print(irState); Serial.print(", ");
  Serial.println(fusedDist);

  count++;
  delay(100);
}
