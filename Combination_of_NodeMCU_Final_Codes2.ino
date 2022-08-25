#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h> 

//#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Dialog 4G 013"                                  
#define WIFI_PASSWORD "F1e692F7"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBn_WUkAnBcLfpY5nj-AQQiOtRncCqgy_Q"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "ravindurcg99@yahoo.com"
#define USER_PASSWORD "FYP2022@ralwis"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "e-scooter-monitoring-app-default-rtdb.asia-southeast1.firebasedatabase.app"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;

String scooterSpeed = "/speed";
String scooterDistance = "/distance";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
int timestamp;

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

// Timer variables (send new readings every 2 sec)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 2000;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup(){
  // Initialize Serial port
  Serial.begin(9600);

  initWiFi();
  timeClient.begin();

  nodemcu.begin(9600);
  while (!Serial) continue;

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(500);
  }

  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}

void loop(){

    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(nodemcu);
  
    if (data == JsonObject::invalid()) {
      //Serial.println("Invalid Json Object");
      jsonBuffer.clear();
      return;
    }
  
    // Send new readings to database
    if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timestamp);

    Serial.println("JSON Object Recieved");
    Serial.print("Recieved Speed:  ");
    float v = data["speed"];
    Serial.println(v);
    Serial.print("Recieved Distance:  ");
    float distance = data["distance"];
    Serial.println(distance);
    Serial.println("-----------------------------------------");

    json.set(scooterSpeed.c_str(), float(v));
    json.set(scooterDistance.c_str(), float(distance));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    
    }
}
