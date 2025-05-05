#include "wifi_module.h"
#include "config.h"
#include <WiFi.h>

void connectToWiFi(const char* ssid, const char* pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.printf("Connecting to Wi-Fi “%s”", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\n✔ Connected! IP: %s   Channel: %d\n",
                WiFi.localIP().toString().c_str(),
                WiFi.channel());
}
