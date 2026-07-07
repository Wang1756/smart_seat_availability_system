#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Servo.h>
#include "HX711.h"

#define WIFI_SSID "" //insert Wi-Fi SSID
#define WIFI_PASSWORD "" //insert Wi-Fi Password

#define API_KEY "AIzaSyDjq8ptsydQSVZeOsaqajkIyprU22eQZrc"
#define DATABASE_URL "mcduduk-default-rtdb.asia-southeast1.firebasedatabase.app"

#define TRIG 5
#define ECHO 18
#define PIR 19
#define SERVO_PIN 13
#define DT 21
#define SCK_PIN 22
#define RED 25
#define GREEN 26
#define BLUE 27

Servo myServo;
HX711 scale;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String current_state = "available";
String last_uploaded = "";
String web_status = "";
unsigned long last_check = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(PIR, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(0); 

  //hx711
  scale.begin(DT, SCK_PIN);
  scale.set_scale(203.04);
  scale.tare(); 

  //wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  //firebase connect
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
  }
}

void loop() {
  if (Firebase.ready() && signupOK) {
    float w = scale.get_units(1); 
    int m = digitalRead(PIR);
    
    digitalWrite(TRIG, LOW); 
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH); 
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    long duration = pulseIn(ECHO, HIGH);
    int d = duration * 0.034 / 2;

    Serial.print("weight: "); Serial.print(w);
    Serial.print(" | motion: "); Serial.print(m);
    Serial.print(" | dist: "); Serial.println(d);

    if (millis() - last_check > 2000) {
      if (Firebase.RTDB.getString(&fbdo, "/tables/t11/status")) {
        web_status = fbdo.stringData();
      }
      last_check = millis();
    }

    //occupied(red)
    if (abs(w) > 6 || m == 1) {
      current_state = "occupied";
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(BLUE, LOW); 
      myServo.write(180);
    } 
    //reserved(blue)
    else if (web_status == "reserved") {
      current_state = "reserved";
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(BLUE, HIGH);
      myServo.write(180);
    }
    //dirty(yellow)
    else if (abs(w) < 25 && d != 2) {
      current_state = "dirty";
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, HIGH);
      digitalWrite(BLUE, LOW);
      myServo.write(0);
    }
    //available(green)
    else {
      current_state = "available";
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(BLUE, LOW);
      myServo.write(0);
    }

    //upload
    if (current_state != last_uploaded && current_state != web_status) {
      if (Firebase.RTDB.setString(&fbdo, "/tables/t11/status", current_state)) {
        last_uploaded = current_state;
      }
    }
  }
  delay(100); 
}