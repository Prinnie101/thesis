#include "relayModule.h"
#include "config.h"
#include <Preferences.h>
#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;  // reference the same MQTT client from your main sketch

static Preferences prefs;

void relayInit(PubSubClient& client) {
  // Ensure the relay pin is configured
  pinMode(RELAY_PIN, OUTPUT);

  // Restore last‐saved state from NVS
  prefs.begin("relayMod", false);
  bool lastOn = prefs.getBool("lastRelay", false);
  digitalWrite(RELAY_PIN, lastOn ? HIGH : LOW);

  Serial.print("[relayInit] Restored relay state: ");
  Serial.println(lastOn ? "ON" : "OFF");

  // Subscribe to incoming MQTT relay commands
  client.subscribe(RELAY_TOPIC, 1);
  Serial.print("[relayInit] Subscribed to topic: ");
  Serial.println(RELAY_TOPIC);
}

void relayCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, RELAY_TOPIC) != 0) return;

  Serial.print("[relayCallback] Received on ");
  Serial.print(topic);
  Serial.print(": ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Determine on/off from payload
  bool on = (length == 1 && payload[0] == '1');

  // Apply to the pin
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  Serial.print("[relayCallback] Relay turned ");
  Serial.println(on ? "ON" : "OFF");

  // Persist new state to NVS
  prefs.putBool("lastRelay", on);
  Serial.println("[relayCallback] State saved to NVS");
}

void relayModuleCommand(bool on) {
  // Ensure pin is configured and set
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);

  // Publish updated state back to broker
  mqttClient.publish(RELAY_STATE_TOPIC, on ? "1" : "0", true);
}
