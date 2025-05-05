//sensorModule.cpp
#include "sensorModule.h"
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>
#include "config.h"

// we’ll reuse the same Serial2 pins
static PZEM004Tv30 pzem(&Serial2, PZEM_RX_PIN, PZEM_TX_PIN);
static unsigned long lastPub = 0;

void sensorInit(PubSubClient& client) {
  // no subscriptions here
}

void sensorLoop(PubSubClient& client) {
  //DELAY < 0
  if (millis() - lastPub < 0) return;
  lastPub = millis();

  StaticJsonDocument<256> doc;
  doc["voltage"]   = pzem.voltage();
  doc["current"]   = pzem.current();
  doc["power"]     = pzem.power();
  doc["energy"]    = pzem.energy();
  doc["frequency"] = pzem.frequency();
  doc["pf"]        = pzem.pf();

  char buf[256];
  size_t n = serializeJson(doc, buf);

  // — CHANGED: capture publish result and print debug indicator —
  bool ok = client.publish(SENSOR_TOPIC, buf, n);
  // if (ok) {
  //   Serial.println("✔️ Sensor data published");
  // } else {
  //   Serial.println("❌ Failed to publish sensor data");
  // }

//   bool ok = client.publish(SENSOR_TOPIC, buf, n);
// if (ok) {
//   Serial.println("✔️ Sensor data published");
//   delay(2000);   // pause 100 ms to let this line stand out
// } else {
//   Serial.println("❌ Failed to publish sensor data");
//   delay(2000);   // same pause on failure
// }

}
