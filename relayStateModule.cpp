//relayStateModule.cpp
#include <Arduino.h>
#include "relayStateModule.h"
#include "config.h"

static unsigned long lastPub = 0;
static const unsigned long INTERVAL = 1000; // publish every 1 s

void relayStateInit(PubSubClient& client) {
  // nothing to subscribe—just publishing
  pinMode(RELAY_PIN, OUTPUT);  // ensure the pin is set to output
}

void relayStateLoop(PubSubClient& client) {
  if (millis() - lastPub < INTERVAL) return;
  lastPub = millis();

  // read the pin: HIGH==1, LOW==0
  int state = digitalRead(RELAY_PIN);
  const char* payload = state ? "1" : "0";

  // publish with retain flag so last-known state is always available
  client.publish(RELAY_STATE_TOPIC, payload, /*retained=*/true);
}
