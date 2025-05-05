#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

// Connects to your AP (WIFI_STA) and blocks until connected,
// then prints IP and channel.
void connectToWiFi(const char* ssid, const char* pass);

#endif  // WIFI_MODULE_H
