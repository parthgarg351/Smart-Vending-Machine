# Smart-Vending-Machine
# Introduction
- In the era of smart technology, integrating IoT (Internet of Things) solutions into everyday devices enhances their functionality and efficiency. This project focuses on designing a smart vending machine using Arduino Mega, equipped with Zigbee modules, RFID readers, servo motors, push buttons, weight sensors, LEDs, and a buzzer. The machine offers an automated, user-friendly experience for customers, allowing seamless item dispensing and real-time data updates to the owner and customer carts via Zigbee communication.

# Components Required

- Arduino Mega
- Arduino Uno
- Zigbee Modules S2C (2)
- RFID Reader MFRC522
- Servo Motors specofoc  (2)
- Push Buttons (3)
- 5kg Weight cell
- LEDs (Green, Red, White, Blue)
- Buzzer
- Breadboard and Jumper Wires
- Power Supply
- 16x2 Alphanumeric Display JHD162A
- 45 ohm potentiometer
- AK XBee USb Adapter FT232RL
- Nano Usb cable(for 14th component)


# Procedure

- Initial Setup:
- Install Arduino IDE on your computer.
- Connect the Arduino Mega to your computer via USB.
# Connecting Components:
- Servo Motors: Connect three servo motors to the Arduino Mega, each controlled by a separate digital pin(5,6,7)
Red Wire - 5v output 
Brown wire- Ground
Yellow Wire- Digital pins
- Push Buttons: Connect three push buttons for dispensing items and one for ending the session.
Item Dispense Button - 22,23,24 (Mega)
Session End Button - 25(Mega)
- NOTE: Connect one end of the push button to the pin mentioned and the other end to the Ground.
- RFID Reader: Connect the RFID reader to the Arduino Mega, ensuring proper communication via serial pins.
SDA - Pin 13 (Mega)
SCK - Pin 52 (Mega)
MOSI - Pin 51 (Mega)
MISO - Pin 50 (Mega) 
RST - Pin 8 (Mega)
- LEDs and Buzzer: Connect LEDs to indicate different states and a buzzer for alerts. (Mega)
Blue- 30
Green -12
Yellow- 4
Red- 31
Buzzer -3
- NOTE: Connect the Anode of these LEDs to the Pins mentioned and connect the Cathode to the Ground
- ZigBee Module (Router): 
Zigbe Pin1 – 3.3 V  (Mega)
Zigbe Pin2 (TX) - pin 19 (RX)  (Mega)
Zigbe Pin3 (RX) - Pin 18 (TX)   (Mega)
Zigbe Pin10 - Ground  (Mega)
- ZigBee Module (Coordinator):
Zigbe Pin1 – 3.3 V  (UNO)
Zigbe Pin2 (TX) - pin 2 (RX)  (UNO)
Zigbe Pin3 (RX) - Pin 3 (TX)  (UNO)
Zigbe Pin10 - Ground  (UNO)
- Display : (Mega)
VO - Middle Pin of Potentiometer
RS - Pin 9
E - Pin 10
D4 - Pin 11
D5 - Pin 2
D6 - Pin A0
D7 - Pin A1
- Potentiometer: Connect 1st and 3rd pin to 5v and to ground. (Mega)
# Coding:
- Arduino Code: Write and upload code to the Arduino Mega to control the servos, read RFID data, manage the session timer, handle button presses, and communicate with the weight sensor.
- Zigbee Configuration:
- Configure the Zigbee modules for coordinator and router roles using XBee Explorer and install XCTU Software
- Using XCTU Software, configure both ZIgbee modules one by one and test if they are communicating properly.
- Ensure proper serial communication between the Arduino Mega and Zigbee modules.
- Node Js Script for Web App:
- Write a Node Js script to receive data from Zigbee and update a web app, displaying the customer and owner carts.
# Testing and Simulation:
- Test each component individually in Tinkercad or a similar simulation tool.
- Verify the integration of all components by simulating the entire vending machine operation.
# Integration:
- Combine all modules and ensure seamless communication between the Arduino Mega and the Arduino Uno via Zigbee.
- Deploy the web app to visualize real-time data updates.

# Problems Faced

- Configuration of Zigbee module was a very hectic task as these modules are very delicate and if one pin gets badly connected u need to update its firmware again and again . Making zigbee communication was a difficult task.
- We need to define custom serial pins for data transmission for Zigbee and we used Software Serial library for configuring that.
- Zigbee sends data character by character, So it becomes difficult to club all characters and concatenate them in a string according to time stamps.
- Weight Sensor needs a lot of calibration and hardware for proper functioning and we were not able to implement it in our project.
- The contrast pin VO of the display was initially difficult to understand afterward we connected it to the potentiometer so that we could adjust its contrast by rotating the nob.

# Project Summary
- This smart vending machine project demonstrates the effective integration of IoT technology to enhance traditional vending machines' functionality. By using Arduino Mega, Zigbee modules, and various sensors and actuators, the machine can autonomously dispense items, validate RFID cards, manage session timings, and update data in real-time. The project showcases the potential of IoT in improving user experience and operational efficiency in everyday applications.
