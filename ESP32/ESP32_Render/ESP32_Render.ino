#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// ==========================================
// 1. HARDWARE DEFINITIONS
// ==========================================
#define PIR_PIN 4 
Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

// ==========================================
// 2. NETWORK CREDENTIALS & RENDER URL
// ==========================================
const char* ssid     = "Sukhoi-57";
const char* password = "99623454569";

// Replace this with your actual Render URL when deployed
const char* SERVER_URL_TELEMETRY = "https://geo-polymer-brick-backend.onrender.com/api/telemetry";
const char* SERVER_URL_COMMAND   = "https://geo-polymer-brick-backend.onrender.com/api/command";

unsigned long lastMillis = 0;
const long interval = 2000; // 2 seconds telemetry interval

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void checkCommands() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(SERVER_URL_COMMAND);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      StaticJsonDocument<200> doc;
      deserializeJson(doc, response);
      const char* cmd = doc["command"];
      
      if (String(cmd) == "REBOOT") {
        Serial.println("WARN: Remote reboot sequence initiated by Web Dashboard...");
        delay(1000);
        ESP.restart();
      }
    }
    http.end();
  }
}

void publishTelemetry() {
  if(WiFi.status() == WL_CONNECTED){
    // 1. Read Sensors
    bool motion = digitalRead(PIR_PIN);
    float airTemp = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F;
    
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // 2. Edge Computing
    bool isQuake = (abs(a.acceleration.x) > 5.0) || (abs(a.acceleration.y) > 5.0);
    bool isStorm = (pressure > 0 && pressure < 1000.0);

    // 3. Build JSON
    StaticJsonDocument<512> doc;
    doc["motionDetected"] = motion;
    doc["outerTemp"] = airTemp;
    doc["pressure"] = pressure;
    doc["accelX"] = a.acceleration.x;
    doc["accelY"] = a.acceleration.y;
    doc["accelZ"] = a.acceleration.z;
    doc["seismicAlert"] = isQuake;
    doc["stormWarning"] = isStorm;

    String jsonString;
    serializeJson(doc, jsonString);

    // 4. POST to Render
    HTTPClient http;
    http.begin(SERVER_URL_TELEMETRY);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(jsonString);
    if (httpResponseCode > 0) {
      Serial.print("Data Pushed to Render. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected. Reconnecting...");
    setupWiFi();
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(PIR_PIN, INPUT);
  if (!mpu.begin()) Serial.println("MPU6050 Fault");
  if (!bmp.begin(0x76)) Serial.println("BMP280 Fault");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  setupWiFi();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;
    publishTelemetry();
    checkCommands();
  }
}
