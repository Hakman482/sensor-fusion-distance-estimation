#define TRIG 2
#define ECHO 3

long duration;
float distance_raw;      // raw sonar distance (cm)
float distance_cal;      // calibrated distance (cm)

int count = 0;          // number of samples taken
int maxCount = 100;      // stop after 50 readings

// Calibration offset (from your Excel analysis)
// d_cal = d_raw - offset
const float SONAR_OFFSET = 0.0203;   // cm

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // ---- CSV header () ----
  // Format: sample,cal_cm
  Serial.println("sample,raw_cm,cal_cm");
}

void loop() {

  // Stop after N samples
  if (count >= maxCount) {
    while (1);   // freeze program
  }

  // ---- Sonar trigger pulse ----
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // ---- Read echo pulse ----
  duration = pulseIn(ECHO, HIGH);

  // ---- Raw distance in cm ----
  distance_raw = duration * 0.034 / 2;

  // ---- Apply calibration offset ----
  distance_cal = distance_raw - SONAR_OFFSET;

  // ---- CSV-style output: sample,raw_cm,cal_cm ----
  Serial.print(count);
  Serial.print(",");
  Serial.print(distance_raw, 3);   // 3 decimal places
  Serial.print(",");
  Serial.println(distance_cal, 3); // newline ends the row

  count++;       // increase sample count
  delay(500);    // small delay between samples
}
