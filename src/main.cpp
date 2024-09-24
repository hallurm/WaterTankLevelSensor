#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// MQTT Broker settings
const char* mqtt_server = "YOUR_MQTT_BROKER";
const int mqtt_port = 1883;
const char* mqtt_topic = "vandtank/niveau";
const char* mqtt_username = "YOUR_MQTT_USERNAME";
const char* mqtt_password = "YOUR_MQTT_PASSWORD";

// Ultrasonic sensor pins
#define TRIGGER_PIN  D1
#define ECHO_PIN     D2
#define MAX_DISTANCE 300 // Maximum distance we want to measure (in centimeters)

// LED strip settings
#define LED_PIN      D4
#define NUM_LEDS     60 // Adjust based on your LED strip

// Create instances
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiClient espClient;
PubSubClient client(espClient);

// Variables
unsigned long lastMeasurementTime = 0;
const unsigned long measurementInterval = 600000; // 10 minutes in milliseconds
int tankHeight = 0; // Will be set during calibration
float waterLevel = 0;
bool isCalibrated = false;

// Function declarations
void setupWiFi();
void reconnectMQTT();
void calibrateTank();
float measureWaterLevel();
void updateLEDStrip(float percentage);
void blinkBottomLED();

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  // Initialize LED strip
  strip.begin();
  strip.show();

  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);

  // Check if tank is calibrated
  EEPROM.get(0, isCalibrated);
  if (!isCalibrated) {
    calibrateTank();
  } else {
    EEPROM.get(4, tankHeight);
  }
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    lastMeasurementTime = currentTime;
    
    float waterLevelPercentage = measureWaterLevel();
    updateLEDStrip(waterLevelPercentage);
    
    // Publish to MQTT
    char msg[8];
    dtostrf(waterLevelPercentage, 1, 2, msg);
    client.publish(mqtt_topic, msg);

    if (waterLevelPercentage == 0) {
      blinkBottomLED();
    }
  }
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void calibrateTank() {
  Serial.println("Calibrating tank height...");
  delay(2000);
  int measurements[5];
  for (int i = 0; i < 5; i++) {
    measurements[i] = sonar.ping_cm();
    delay(1000);
  }
  tankHeight = 0;
  for (int i = 0; i < 5; i++) {
    tankHeight += measurements[i];
  }
  tankHeight /= 5;
  
  EEPROM.put(0, true);
  EEPROM.put(4, tankHeight);
  EEPROM.commit();
  
  Serial.print("Tank height calibrated to: ");
  Serial.print(tankHeight);
  Serial.println(" cm");
}

float measureWaterLevel() {
  int distance = sonar.ping_cm();
  waterLevel = tankHeight - distance;
  float percentage = (waterLevel / tankHeight) * 100;
  percentage = constrain(percentage, 0, 100);
  return percentage;
}

void updateLEDStrip(float percentage) {
  int ledsToLight = map(percentage, 0, 100, 0, NUM_LEDS);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledsToLight) {
      if (percentage > 50) {
        strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
      } else if (percentage > 15) {
        strip.setPixelColor(i, strip.Color(255, 255, 0)); // Yellow
      } else {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
      }
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Off
    }
  }
  
  strip.setBrightness(map(percentage, 0, 100, 20, 255));
  strip.show();
}

void blinkBottomLED() {
  for (int i = 0; i < 5; i++) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
    delay(500);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
    delay(500);
  }
}
