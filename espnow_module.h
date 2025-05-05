 #pragma once 
 #include <esp_now.h>
 #include <WiFi.h>

 // payload type header byte
 enum PayloadType : uint8_t {
   PT_CREDIT = 0x01,
   PT_RELAY  = 0x02,
 };

 // common payload struct
 #pragma pack(push, 1)
 struct Payload {
   PayloadType type;
   float value;
 };
 #pragma pack(pop)

 // init and peer registration

 // initialize ESP-NOW, syncing to the current Wi-Fi channel
 void initEspNow();

 // add a peer; channel will be set to current Wi-Fi channel
 void registerPeer(const uint8_t mac[6]);

 // send helper
 // send with debug dump
 esp_err_t sendPayload(const Payload* p);

 // if you ever need to re-sync (e.g. channel change), call this
 void syncEspNowToWiFiChannel();

 // user dispatch callbacks to implement
 void onCreditReceived(float credit);
 void onRelayReceived(bool on);
