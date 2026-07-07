# Smart Seat Availability Detection System

This repository contains the code and configuration for an IoT project designed to automate the monitoring of shared seating spaces like libraries and cafeterias. It utilizes data fusion from weight and motion sensors to accurately determine seat status, preventing false alarms, and includes a cleanliness monitoring feature.

# Features
- Live occupancy detection displaying states like Occupied, Available, Dirty, and Reserved.
- Data fusion logic combining physical weight verification with motion tracking.
- Obstacle detection to identify empty but cluttered tables that require cleaning.
- Live HTML web dashboard synced instantly via Firebase.
- Physical status indicators using an RGB LED and Servo Motor.

# Hardware Requirements
- NodeMCU ESP32 Microcontroller
- 10kg Load Cell and HX711 Amplifier
- HC-SR501 PIR Motion Sensor
- HC-SR04 Ultrasonic Sensor
- RGB LED (Cathode)
- SG90 Microservo
- Breadboards and Jumper Wires

# Software Requirements
- Arduino IDE
- Google Firebase
- Web Browser

# Setup Instructions
1. Hardware Assembly
Connect the Load Cell via the HX711, the HC-SR501 PIR sensor, the HC-SR04 Ultrasonic sensor, the RGB LED, and the SG90 Servo to the exact GPIO pins defined in the ESP32 code.

2. Firebase Configuration
	1. Go to the Firebase Console and create a new project.
	2. Create a Database.
	3. Set the database rules to allow read and write access for testing.
	4. Note down your API Key, Database URL, and Project ID.

3. Microcontroller Setup
	1. Open the Arduino IDE.
	2. Install the ESP32 board package and required Firebase libraries.
	3. Open the C++ sketch.
	4. Update the WiFi credentials and Firebase configuration details in the code.
	5. Upload the code to the ESP32.

4. Web Dashboard Setup
	1. Open the HTML file in a text editor.
	2. Replace the firebaseConfig variables with your project details.
	3. Open the HTML file in a web browser to view the live dashboard.
