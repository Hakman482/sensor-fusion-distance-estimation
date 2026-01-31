// IR Reflective Sensor - Digital Object Detection
// DOUT must be connected to a digital pin 

int irPin = 4;    
int count = 0;
int maxCount = 20;

void setup() {
  pinMode(irPin, INPUT);
  Serial.begin(9600);
  Serial.println("IR Reflective Sensor Detection Test Started...");
}

void loop() {

  if (count >= maxCount) {
    Serial.println("Finished IR detection test.");
    while (1);  // stop the program
  }

  int state = digitalRead(irPin);

  Serial.print("Sample ");
  Serial.print(count);
  Serial.print(": ");

  if (state == LOW) {
    Serial.println("Detected");     // object is close / reflective
  } 
  else {
    Serial.println("Not Detected"); // no reflection / object far
  }

  count++;
  delay(300);
}
