// config.h 
#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// — Wi-Fi & MQTT —
static const char* SSID        = "Cyber_Enthusiast";
static const char* WIFI_PASS   = "Cyber_Enthusiast@";
static const char* MQTT_SERVER = "srv782321.hstgr.cloud";
static const int   MQTT_PORT   = 1883;

// — Topics —
static const char* SENSOR_TOPIC      = "boardinghouse/sensor/data";
static const char* RELAY_TOPIC       = "boardinghouse/relay/command";
static const char* CREDIT_TOPIC      = "boardinghouse/credit/1";
static const char* RELAY_STATE_TOPIC = "boardinghouse/relay/state";

// — Pins —
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define RELAY_PIN   18

// ─── ESP-NOW Peers ───────────────────────────────────────────────
// Number of peers you’ll be talking to via ESP-NOW
static const size_t NUM_PEERS = 1;

// The MAC address(es) of each peer (6 bytes each).
// Replace with your actual peer’s MAC(s).
static const uint8_t PEER_MACS[NUM_PEERS][6] = {
  { 0x24, 0x6F, 0x28, 0xBD, 0x47, 0x68 }
};

#endif // CONFIG_H
