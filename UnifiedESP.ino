#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

#include "wifi_module.h"
#include "espnow_module.h"
#include "config.h"
#include "sensorModule.h"
#include "creditModule.h"
#include "relayModule.h"
#include "offlineCreditModule.h"
#include "offlineRelayModule.h"

// timing for ESPN-OW transmissions
static unsigned long lastEspNowTx = 0;
const unsigned long ESP_NOW_TX_INTERVAL = 5000; // ms

// Globals
WiFiClient       wifiClient;
PubSubClient     mqttClient(wifiClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  // 1) Bring up Wi-Fi (locks in channel for ESP-NOW)
  connectToWiFi(SSID, WIFI_PASS);

  // 2) Init ESP-NOW & register peers
  initEspNow();
  for (size_t i = 0; i < NUM_PEERS; ++i) {
    registerPeer(PEER_MACS[i]);
  }

  // 3) Initialize your online modules
  creditInit(mqttClient, lcd);
  relayInit(mqttClient);
  sensorInit(mqttClient);

  // 4) Initialize your offline modules
  offlineCreditModuleInit(lcd);
  offlineRelayModuleInit();
}

void loop() {
  bool online = (WiFi.status() == WL_CONNECTED && mqttClient.connected());
  
  if (online) {
    // — ONLINE PATH via MQTT —
    mqttClient.loop();
    creditLoop(lcd);
    // relayLoop();
    sensorLoop(mqttClient);

  } else {
    // — OFFLINE PATH via ESP-NOW —
    offlineCreditModuleLoop(lcd);
    offlineRelayModuleLoop();
    sensorLoop(mqttClient);

    // Periodically broadcast current credit over ESP-NOW
    if (millis() - lastEspNowTx >= ESP_NOW_TX_INTERVAL) {
      Payload p;
      p.type  = PT_CREDIT;
      p.value = getStoredCredit();
      sendPayload(&p);
      lastEspNowTx = millis();
    }
  }
}

// ------------------------------------------------------------------
// Callback hooks for incoming ESP-NOW packets
// ------------------------------------------------------------------

// Called by espnow_module when a credit packet arrives
 void onCreditReceived(float credit) {
   // pass our global lcd into the module
   creditModuleUpdate(credit, lcd);
 }

// Called by espnow_module when a relay-command packet arrives
void onRelayReceived(bool on) {
  relayModuleCommand(on);
}
