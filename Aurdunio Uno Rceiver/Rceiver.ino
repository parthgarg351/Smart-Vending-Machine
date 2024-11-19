#include <SoftwareSerial.h>

// Define the pins for SoftwareSerial communication (use pins 2 and 3 here)
SoftwareSerial mySerial(2, 3); // RX, TX (Connect RX of Zigbee to pin 3 and TX to pin 2)

const long baudRate = 9600;  // Baud rate for Zigbee module (adjust if needed)

void setup() {
  // Start communication with Serial Monitor (Output screen)
  Serial.begin(9600);

  // Start communication with Zigbee module via SoftwareSerial
  mySerial.begin(baudRate);

  // Print a welcome message to indicate the system is ready
  Serial.println("Zigbee Data Receiver Ready!");
}

void loop() {
  // Check if data is available from the Zigbee module
  if (mySerial.available()) {
    // Read the incoming byte and send it to the Serial Monitor
    char receivedChar = mySerial.read();
    //Serial.print("Received Data: "); // Prefix text
    Serial.println(receivedChar);    // Display the data received from Zigbee
  }

  // Optionally, send data from the Serial Monitor to the Zigbee module
  if (Serial.available()) {
    // Read the byte from Serial Monitor and send it to the Zigbee module
    char dataToSend = Serial.read();
    mySerial.print(dataToSend);  // Send data to Zigbee
  }
}
