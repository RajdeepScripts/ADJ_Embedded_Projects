//Railway Track Fault Detection System
//Done on a Arduino Uno Rev 3 
//Rajdeep Mishra

#include <Wire.h>

#define ADXL345_ADDR 0x53  // I2C address for ADXL345

#define vibrationPin 2 
#define irPin 3
#define buzzer 8
#define led 9

// Threshold values (tune as per setup)
const int vibrationThreshold = 200;   // for SW-420 (digital or analog)
const float tiltThreshold = 10.0;     // degrees
int irState = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // --- Initialize ADXL345 ---
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x2D);  // Power Control register
  Wire.write(0x08);  // Set the Measure bit to begin measurement
  Wire.endTransmission();

  pinMode(vibrationPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.println("🚆 Railway Track Fault Detection System Initialized (ADXL345 Version)");
  delay(1000);
}

void loop() {
  // --- 1. Read Vibration Sensor ---
  int vibrationState = digitalRead(vibrationPin);

  // --- 2. Read IR Sensor ---
  irState = digitalRead(irPin);

  // --- 3. Read Accelerometer from ADXL345 ---
  int16_t ax, ay, az;
  readAccelerometer(ax, ay, az);

  // Convert to tilt angles (in degrees)
  float angleX = 0, angleY = 0;
  float denomX = sqrt(pow(ay, 2) + pow(az, 2));
  float denomY = sqrt(pow(ax, 2) + pow(az, 2));

  // Avoid division by zero
  if (denomX != 0 && denomY != 0) {
    angleX = atan(ax / denomX) * 57.2958;
    angleY = atan(ay / denomY) * 57.2958;
  }

  Serial.print("Vibration: "); Serial.print(vibrationState);
  Serial.print(" | IR: "); Serial.print(irState);
  Serial.print(" | Tilt X: "); Serial.print(angleX, 2);
  Serial.print(" | Tilt Y: "); Serial.println(angleY, 2);

  bool faultDetected = false;

  // --- Fault Conditions ---
  if (vibrationState == HIGH) {
    Serial.println("⚠ Abnormal vibration detected!");
    faultDetected = true;
  }

  if (abs(angleX) > tiltThreshold || abs(angleY) > tiltThreshold) {
    Serial.println("⚠ Track tilt/displacement detected!");
    faultDetected = true;
  }

  if (irState == LOW) {
    Serial.println("⚠ Track obstacle or gap detected!");
    faultDetected = true;
  }

  // --- Alarm Section ---
  if (faultDetected) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
    delay(1000);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }

  delay(500);
}

// --- Function to Read Data from ADXL345 ---
void readAccelerometer(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);  // Data registers starting from 0x32
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6, true);

  if (Wire.available() == 6) {
    x = (Wire.read() | (Wire.read() << 8));
    y = (Wire.read() | (Wire.read() << 8));
    z = (Wire.read() | (Wire.read() << 8));
  } else {
    x = y = z = 0;  // Prevent NaN by defaulting to 0 if read fails
  }
}
