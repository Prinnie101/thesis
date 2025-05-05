#include "offlineRelayModule.h"
#include "config.h"
#include <Preferences.h>
#include <Arduino.h>

static Preferences prefs;
static unsigned long lastUpdateRelay = 0;

void offlineRelayModuleInit() {
  prefs.begin("relayMod", false);
  bool on = prefs.getBool("lastRelay", false);
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

void offlineRelayModuleLoop() {
  // re-enforce every 100 ms (in case of external noise)
  if (millis() - lastUpdateRelay < 100) return;
  lastUpdateRelay = millis();

  bool on = prefs.getBool("lastRelay", false);
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}
