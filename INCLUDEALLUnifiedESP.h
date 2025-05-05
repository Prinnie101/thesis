#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

#include "sensorModule.h"
#include "relayModule.h"
#include "creditModule.h"

// — Globals —
WiFiClient     wifiClient;
PubSubClient   mqttClient(wifiClient);
LiquidCrystal_I2C lcd(0x27,16,2);

void connectWiFi() {
  WiFi.begin(SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  relayCallback(topic, payload, length);
  creditCallback(topic, payload, length);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Unified")) {
      relayInit(mqttClient);
      creditInit(mqttClient, lcd);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  connectWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  sensorInit(mqttClient);
}

void loop() {
  if (!mqttClient.connected()) reconnectMQTT();
  mqttClient.loop();

  sensorLoop(mqttClient);
  creditLoop(lcd);
}
