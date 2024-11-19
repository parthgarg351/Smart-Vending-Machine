#include <LiquidCrystal.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         8          // Configurable, see typical pin layout above
#define SS_PIN          13         // Configurable, see typical pin layout above

// Pin assignments
const int buttonDispensePin1 = 22; // Button to dispense item 1
const int buttonDispensePin2 = 23; // Button to dispense item 2
const int buttonDispensePin3 = 24; // Button to dispense item 3
const int buttonEndSessionPin = 25; // Button to end session

const int ledBluePin = 30;    // Idle state indicator
const int ledGreenPin = 12;  // Access granted indicator
const int ledRedPin = 31;    // Access denied indicator
const int ledYellowPin = 4;  // Item waiting indicator
const int buzzerPin = 3;     // Buzzer

// Servo pins
const int servo1Pin = 5;
const int servo2Pin = 6;
const int servo3Pin = 7;

Servo servo1;
Servo servo2;
Servo servo3;

// LCD Pins
LiquidCrystal lcd(9, 10, 11, 2, A0, A1);  // RS, E, D4, D5, D6, D7
const int ct =46;

// RFID setup
// SDA Pin - 13, SCK Pin - 52, MOSI Pin - 51, MISO Pin - 50, Reset Pin - 8
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Weight sensor setup
const int weightSensorPin = A2;  // Pin for weight sensor (analog input)
int weightValue = 0;  // Variable to store weight sensor data

// Session state and timers
bool sessionActive = false;
unsigned long sessionStartTime;
unsigned long pickupWaitStartTime;
bool itemCheck=false;

void setup() {
  analogWrite(ct, 250);

  // Initialize Serial for debugging and RFID communication
  Serial.begin(9600);
  SPI.begin();  // Initialize SPI bus for RFID
  mfrc522.PCD_Init();  // Initialize RFID reader

  // Initialize servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  // Set up LEDs and buzzer
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Set up push buttons
  pinMode(buttonDispensePin1, INPUT_PULLUP);
  pinMode(buttonDispensePin2, INPUT_PULLUP);
  pinMode(buttonDispensePin3, INPUT_PULLUP);
  pinMode(buttonEndSessionPin, INPUT_PULLUP);

  // Set up weight sensor
  pinMode(weightSensorPin, INPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  displayIdleState();

  // Set initial LED states
  digitalWrite(ledBluePin, HIGH); // Idle indicator
  digitalWrite(ledGreenPin, LOW);
  digitalWrite(ledRedPin, LOW);
  digitalWrite(ledYellowPin, LOW);
}

void loop() {
  // Check RFID tag
  if (!sessionActive) {
    if (checkRFID()) {
      startSession();
    } else {
      denyAccess();
    }
  }

  // Dispense item if corresponding button is pressed
  if (sessionActive && digitalRead(buttonDispensePin1) == LOW) {
    dispenseItem(servo1);
  }
  if (sessionActive && digitalRead(buttonDispensePin2) == LOW) {
    dispenseItem(servo2);
  }
  if (sessionActive && digitalRead(buttonDispensePin3) == LOW) {
    dispenseItem(servo3);
  }

  // End session if button pressed
  if (sessionActive && digitalRead(buttonEndSessionPin) == LOW) {
    endSession();
  }

  // Check if item is picked up
  if (sessionActive && checkItemPickup()) {
    itemPickedUp();
  }

  // Check if session has expired
  if (sessionActive && millis() - sessionStartTime > 120000) {
    endSession();
  }
}

// Functions

void displayIdleState() {
  lcd.clear();
  lcd.print("Vending Machine");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
}

void startSession() {
  sessionActive = true;
  sessionStartTime = millis();
  
  digitalWrite(ledBluePin, LOW);
  digitalWrite(ledGreenPin, HIGH);

  lcd.clear();
  lcd.print("Access Granted!");
  lcd.setCursor(0, 1);
  lcd.print("Session Active");
  delay(1000);
  Serial.println("Session started");  // Send data to Raspberry Pi
}

void denyAccess() {
  digitalWrite(ledRedPin, HIGH);
  lcd.clear();
  lcd.print("Access Denied!");
  delay(2000);  // Show for 2 seconds
  digitalWrite(ledRedPin, LOW);
  displayIdleState();
}

void dispenseItem(Servo &servo) {
  // Activate servo to dispense item
  servo.write(90);  // Move to dispensing position
  delay(500);        // Give time for dispensing
  servo.write(0);   // Move back to default position

  // Activate item waiting indicator
  digitalWrite(ledYellowPin, HIGH);
  pickupWaitStartTime = millis();

  lcd.clear();
  lcd.print("Item Dispensed!");
  lcd.setCursor(0, 1);
  lcd.print("Pick it up.");

  Serial.println("Item dispensed");  // Send data to Raspberry Pi
}

void itemPickedUp() {
  digitalWrite(ledYellowPin, LOW);

  lcd.clear();
  lcd.print("Item Picked!");
  delay(1000); // Display briefly
  lcd.clear();
  lcd.print("Session Active");

  Serial.println("Item picked");  // Send data to Raspberry Pi
}

bool checkRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String rfid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfid += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("RFID Detected: " + rfid);
    return true;  // Assuming the RFID is valid
  }
  return false;
}

bool checkItemPickup() {
  weightValue = analogRead(weightSensorPin);
  if (weightValue > 100) {  // Adjust the threshold value based on sensor calibration
    return true;
  }
  return false;
}

void endSession() {
  sessionActive = false;
  digitalWrite(ledGreenPin, LOW);
  digitalWrite(ledBluePin, HIGH);

  lcd.clear();
  lcd.print("Session Ended");
  delay(2000);
  displayIdleState();

  // Signal end of session with buzzer
  tone(buzzerPin, 1000, 500); // Beep
  delay(500);
  tone(buzzerPin, 1000, 500); // Beep again

  Serial.println("Session ended");  // Send data to Raspberry Pi
}
