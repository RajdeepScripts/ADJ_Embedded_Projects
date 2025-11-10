//Railway Environment and Direction detection project


#include <Wire.h>
#include "DFRobot_BMM150.h"
#include <DHT.h>

// -------------------- Pin Definitions --------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define LDR_PIN A0
#define BUZZER 8
#define LED 9

// -------------------- Sensor Objects --------------------
DHT dht(DHTPIN, DHTTYPE);
DFRobot_BMM150_I2C bmm150(&Wire, I2C_ADDRESS_4); // Using I2C interface

// -------------------- Setup Function --------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  dht.begin();
  Wire.begin();

  // Initialize the BMM150 magnetometer
  while (bmm150.begin()) {
    Serial.println("❌ BMM150 init failed! Check wiring...");
    delay(1000);
  }

  Serial.println("✅ BMM150 initialized successfully!");

  // Configure magnetometer
  bmm150.setOperationMode(BMM150_POWERMODE_NORMAL);
  bmm150.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
  bmm150.setRate(BMM150_DATA_RATE_10HZ);
  bmm150.setMeasurementXYZ();

  Serial.println("✅ Smart Railway Track Condition & Environment Monitoring System Started");
}

// -------------------- Main Loop --------------------
void loop() {
  // --- Magnetometer Readings ---
  sBmm150MagData_t magData = bmm150.getGeomagneticData();
  float magX = magData.x;
  float magY = magData.y;
  float magZ = magData.z;

  // Calculate total magnetic field
  float magneticField = sqrt(magX * magX + magY * magY + magZ * magZ);
  float compassDegree = bmm150.getCompassDegree();

  // --- DHT11 Readings ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Light Sensor Reading ---
  int lightValue = analogRead(LDR_PIN);
  int lightPercent = map(lightValue, 0, 1023, 0, 100);

  // --- Display Data ---
  Serial.println("=======================================");
  Serial.print("Magnetic Field Strength: "); Serial.print(magneticField); Serial.println(" µT");
  Serial.print("Compass Direction: "); Serial.print(compassDegree); Serial.println("°");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Light Intensity: "); Serial.print(lightPercent); Serial.println(" %");
  Serial.println("---------------------------------------");

  // --- Fault Detection Logic ---
  bool fault = false;

  // 1️⃣ Track magnetic disturbance
  if (magneticField > 90 || magneticField < 20) {
    Serial.println("⚠ ALERT: Track Magnetic Disturbance Detected!");
    fault = true;
  }

  // 2️⃣ Unsafe environmental condition
  if (temperature > 45 || humidity > 85) {
    Serial.println("⚠ ALERT: Unsafe Environmental Conditions!");
    fault = true;
  }

  // 3️⃣ Low light / visibility
  if (lightPercent < 15) {
    Serial.println("⚠ ALERT: Low Visibility Detected!");
    fault = true;
  }

  // --- Alert Handling ---
  if (fault) {
    digitalWrite(LED, HIGH);
    tone(BUZZER, 1000);
  } else {
    digitalWrite(LED, LOW);
    noTone(BUZZER);
  }

  delay(2000); // update every 2 seconds
}
