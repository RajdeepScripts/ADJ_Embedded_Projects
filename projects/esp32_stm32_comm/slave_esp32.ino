String incomingData = ""; // buffer to store incoming bytes

// Change these pins if using different connections
#define RX_PIN 3   // Connect this to STM32 TX (PA9)
#define TX_PIN 1   // Connect this to STM32 RX (PA10)

void setup() {
  Serial.begin(115200);                      // USB monitor
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);  // UART1 with chosen pins

  Serial.println("ESP32-CAM UART1 SLAVE READY");
}

void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();  // read incoming byte

    if (c == '\n') {
      // End of message → print complete message
      Serial.print("Received from STM32: ");
      Serial.println(incomingData);

      // Optionally, send ACK back to STM32
      Serial1.print("ESP32_ACK: ");
      Serial1.println(incomingData);

      incomingData = ""; // clear buffer
    } else {
      incomingData += c; // append byte to buffer
    }
  }
}
