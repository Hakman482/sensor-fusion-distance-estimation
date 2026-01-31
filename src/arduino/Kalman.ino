#include <SoftwareSerial.h>
#include <math.h>

#define TRIG 2
#define ECHO 3
#define IR_PIN 4

SoftwareSerial lidarSerial(8, 9);

int maxCount = 200;
int count = 0;

int lastValidLidar = 15;

// =============================
// KALMAN VARIABLES
// =============================
float x = 0;        // estimated distance (will be initialised from first measurement)
float P = 50.0;    // covariance (large so filter trusts measurements initially)
float Q = 0.1;      // process noise

float R_lidar = 4.0;
float R_sonar = 9.0;

float beta = 0.90;

// Start-up control
bool kfInit = false;
int warmupSamples = 10;   // during warmup, do NOT adapt R
int warmupCount = 0;

// =============================
// KALMAN UPDATE
// =============================
float kalmanAdaptiveUpdate(float z, float &R_sensor, bool adaptR) {

  // Predict
  float x_pred = x;
  float P_pred = P + Q;

  // Innovation
  float innovation = z - x_pred;

  // Optional: adapt measurement noise AFTER warmup only
  if (adaptR) {
    float inst_noise = innovation * innovation;

    // Bound inst_noise to avoid absurd R growth from occasional outliers
    // (tune these bounds to your setup)
    if (inst_noise < 0.01f) inst_noise = 0.01f;
    if (inst_noise > 400.0f) inst_noise = 400.0f; // e.g., 20cm^2

    R_sensor = beta * R_sensor + (1 - beta) * inst_noise;

    if (R_sensor < 0.1f)  R_sensor = 0.1f;
    if (R_sensor > 500.0f) R_sensor = 500.0f;
  }

  // Update
  float K = P_pred / (P_pred + R_sensor);
  x = x_pred + K * innovation;
  P = (1 - K) * P_pred;

  return x;
}

// =============================
// SONAR & LIDAR READERS
// =============================
float readSonar() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Add timeout to prevent blocking and to detect invalid readings
  unsigned long duration = pulseIn(ECHO, HIGH, 30000UL); // 30ms timeout

  if (duration == 0) return NAN; // invalid / timeout

  float d = duration * 0.034f / 2.0f;

  // Basic sanity bounds (tune to your experiment range)
  if (d <= 0.5f || d > 300.0f) return NAN;

  return d;
}

int readLidar() {
  if (lidarSerial.available() >= 9) {
    if (lidarSerial.read() == 0x59 && lidarSerial.read() == 0x59) {

      int low  = lidarSerial.read();
      int high = lidarSerial.read();
      int dist = low + high * 256;

      for (int i = 0; i < 5; i++) lidarSerial.read();

      if (dist > 0 && dist < 300) {
        lastValidLidar = dist;
        return dist;
      }
    }
  }
  return lastValidLidar;
}

// =============================
// SETUP
// =============================
void setup() {
  Serial.begin(9600);
  lidarSerial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_PIN, INPUT);

  Serial.println("sample,sonar,lidar,fused");
}

// =============================
// MAIN LOOP
// =============================
void loop() {

  if (count >= maxCount) {
    Serial.println("Done.");
    while (1);
  }

  float sonarDist = readSonar();
  int   lidarDist = readLidar();
  int   irState   = digitalRead(IR_PIN); // logged only (no influence)

  // ---- Initialise filter from the first reliable measurement ----
  if (!kfInit) {
    x = (float)lidarDist;  // or use average if sonar valid: x = (isfinite(sonarDist)? (sonarDist+lidarDist)/2 : lidarDist);
    P = 100.0f;
    warmupCount = 0;
    kfInit = true;
  }

  // During warmup, do not adapt R (prevents R blowing up due to initial mismatch)
  bool adaptR = (warmupCount >= warmupSamples);

  // Update with valid sonar only
  if (isfinite(sonarDist)) {
    kalmanAdaptiveUpdate(sonarDist, R_sonar, adaptR);
  }

  // Always update with lidar (you already hold lastValidLidar)
  kalmanAdaptiveUpdate((float)lidarDist, R_lidar, adaptR);

  float fused = x;

  // CSV OUTPUT
  Serial.print(count);      Serial.print(",");
  Serial.print(sonarDist);  Serial.print(",");
  Serial.print(lidarDist);  Serial.print(",");
  Serial.println(fused);

  count++;
  if (warmupCount < 1000000) warmupCount++;
  delay(120);
}
