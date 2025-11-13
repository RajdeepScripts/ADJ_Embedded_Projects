//Cloud_Based_Data_Upload
//Rajdeep Mishra & Anubhuti Pattanaik


#define BLYNK_TEMPLATE_ID "TMPL3FWQ7-ZIs"
#define BLYNK_TEMPLATE_NAME "cloud"
#define BLYNK_AUTH_TOKEN "bz6HdWNrQ5e4BAQh0qeOYkeuY5jdIB16"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>   // DHT sensor library

// ===== Wi-Fi Credentials =====
char ssid[] = "Adj engineering";
char pass[] = "ADJ@1234";

// ===== LED Pins =====
int ledPin = 2;   // On-board LED (GPIO 2)
int LEDPIN = 5;   // External LED (GPIO 5)

// ===== DHT11 Setup =====
#define DHTPIN 4        // DHT11 DATA pin -> GPIO 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== Ultrasonic Sensor Setup =====
#define TRIG_PIN 12     // Trigger pin -> GPIO 12
#define ECHO_PIN 13     // Echo pin -> GPIO 13

// ===== Virtual Pins =====
#define VPIN_LED V0
#define VPIN_LED2 V1
#define VPIN_TEMP V2
#define VPIN_HUMIDITY V3
#define VPIN_DISTANCE V4

BlynkTimer timer;

// ===== BLYNK LED Control =====
BLYNK_WRITE(VPIN_LED)
{
  int ledState = param.asInt(); 
  digitalWrite(ledPin, ledState);
  Serial.print("LED1 set to: ");
  Serial.println(ledState ? "ON" : "OFF");
}

BLYNK_WRITE(VPIN_LED2)
{
  int ledState1 = param.asInt(); 
  digitalWrite(LEDPIN, ledState1);
  Serial.print("LED2 set to: ");
  Serial.println(ledState1 ? "ON" : "OFF");
}

// ===== Function to Send DHT11 Data =====
void sendDHTData()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Blynk.virtualWrite(VPIN_TEMP, temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
}

// ===== Function to Send Ultrasonic Distance =====
void sendUltrasonicData()
{
  long duration;
  float distanceCm;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * 0.0343 / 2;  // Convert to cm

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  Blynk.virtualWrite(VPIN_DISTANCE, distanceCm);
}

// ===== Setup =====
void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(LEDPIN, LOW);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  Serial.println("Connecting to Blynk Cloud...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

 #define BLYNK_TEMPLATE_ID "TMPL3FWQ7-ZIs"
#define BLYNK_TEMPLATE_NAME "cloud"
#define BLYNK_AUTH_TOKEN "bz6HdWNrQ5e4BAQh0qeOYkeuY5jdIB16"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>   // DHT sensor library

// ===== Wi-Fi Credentials =====
char ssid[] = "Adj engineering";
char pass[] = "ADJ@1234";

// ===== LED Pins =====
int ledPin = 2;   // On-board LED (GPIO 2)
int LEDPIN = 5;   // External LED (GPIO 5)

// ===== DHT11 Setup =====
#define DHTPIN 4        // DHT11 DATA pin -> GPIO 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== Ultrasonic Sensor Setup =====
#define TRIG_PIN 12     // Trigger pin -> GPIO 12
#define ECHO_PIN 13     // Echo pin -> GPIO 13

// ===== Virtual Pins =====
#define VPIN_LED V0
#define VPIN_LED2 V1
#define VPIN_TEMP V2
#define VPIN_HUMIDITY V3
#define VPIN_DISTANCE V4

BlynkTimer timer;

// ===== BLYNK LED Control =====
BLYNK_WRITE(VPIN_LED)
{
  int ledState = param.asInt(); 
  digitalWrite(ledPin, ledState);
  Serial.print("LED1 set to: ");
  Serial.println(ledState ? "ON" : "OFF");
}

BLYNK_WRITE(VPIN_LED2)
{
  int ledState1 = param.asInt(); 
  digitalWrite(LEDPIN, ledState1);
  Serial.print("LED2 set to: ");
  Serial.println(ledState1 ? "ON" : "OFF");
}

// ===== Function to Send DHT11 Data =====
void sendDHTData()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Blynk.virtualWrite(VPIN_TEMP, temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
}

// ===== Function to Send Ultrasonic Distance =====
void sendUltrasonicData()
{
  long duration;
  float distanceCm;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * 0.0343 / 2;  // Convert to cm

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  Blynk.virtualWrite(VPIN_DISTANCE, distanceCm);
}

// ===== Setup =====
void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(LEDPIN, LOW);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  Serial.println("Connecting to Blynk Cloud...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send sensor data every 2 seconds
  timer.setInterval(2000L, sendDHTData);
  timer.setInterval(1500L, sendUltrasonicData);
}

// ===== Loop =====
void loop()
{
  Blynk.run();
  timer.run();
} // Send sensor data every 2 seconds
  timer.setInterval(2000L, sendDHTData);
  timer.setInterval(1500L, sendUltrasonicData);
}

// ===== Loop =====
void loop()
{
  Blynk.run();
  timer.run();
}
