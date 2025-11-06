//Mini Radar System Using Ardiuno Uno


#include <Servo.h>
#define trigPin 10
#define echoPin 11
#define buzzer 8

Servo servoMotor;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  servoMotor.attach(9);
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    servoMotor.write(angle);
    delay(5);
    distance = getDistance();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 0 && distance <= 20) {  // Object within 20 cm
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    }
  }

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    servoMotor.write(angle);
    delay(5);
    
    distance = getDistance();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 0 && distance <= 20) {
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    }
  }
}

// Function to get distance from Ultrasonic Sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}
