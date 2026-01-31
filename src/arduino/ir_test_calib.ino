// IR Reflective Sensor Distance Test (Analog)
// AOUT must be connected to A0

int analogPin = A0;
int count = 0;
int maxCount = 100;

void setup() {
  Serial.begin(9600);
  Serial.println("Sample,Distance_cm");  // CSV header (optional)
}

void loop() {

  if (count >= maxCount) {
    Serial.println("Finished taking IR distance readings.");
    while (1);  // stop
  }

  // Read analog value (0–1023)
  int raw = analogRead(analogPin);

  // Convert to voltage (0–5V)
  float voltage = raw * (5.0 / 1023.0);

  // VERY APPROXIMATE distance mapping (depends on reflectivity)
  // You can calibrate this by measuring real distances.
 // float distance = 25.0 * (1.0 / voltage);  
 float distance = 11.458 * pow(voltage, 0.6704);;
  // <-- inverse relation is typical for IR reflective sensors



    // Print CSV-friendly output: SampleNumber,Distance
  Serial.print(count);
  Serial.print(",");
  Serial.println(distance, 3);


  count++;
  delay(500);
}

