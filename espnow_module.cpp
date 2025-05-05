#include <WiFi.h>
#include <esp_wifi.h>            // for esp_wifi_set_channel()
#include <esp_now.h>
#include <string.h>              // for memcpy
#include "espnow_module.h"
#include "config.h"              // for PEER_MACS, NUM_PEERS

// ——— Sync ESP-NOW radio to match your STA Wi-Fi channel ———
void syncEspNowToWiFiChannel() {
  uint8_t ch = WiFi.channel();
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  Serial.printf("ESPNOW → synced to Wi-Fi channel %d\n", ch);
}

// Called by the ESP-NOW internals whenever a packet arrives
static void onDataRecv(const esp_now_recv_info_t* info,
                       const uint8_t* data, int len) {
  if (len < sizeof(Payload)) return;

  // 1) who & count
  Serial.printf("<<< ESPNOW Recv %d bytes from %02X:%02X:%02X:%02X:%02X:%02X\n",
    len,
    info->src_addr[0],info->src_addr[1],info->src_addr[2],
    info->src_addr[3],info->src_addr[4],info->src_addr[5]
  );

  // 2) raw dump (hex)
  Serial.print("    Raw (hex): ");
  for (int i = 0; i < len; i++) Serial.printf("%02X ", data[i]);
  Serial.println();

  // 3) raw dump (ascii)
  Serial.print("    Raw (ascii): \"");
  Serial.write(data, len);
  Serial.println("\"");

  // 4) parse into your Payload struct
  const Payload* p = (const Payload*)data;
  switch (p->type) {
    case PT_CREDIT:
      Serial.printf("    → CREDIT: %.3f\n", p->value);
      onCreditReceived(p->value);
      break;
    case PT_RELAY:
      Serial.printf("    → RELAY : %s\n",
        p->value > 0.5f ? "ON" : "OFF");
      onRelayReceived(p->value > 0.5f);
      break;
    default:
      Serial.printf("    → Unknown type 0x%02X\n", p->type);
  }
}

// (Optional) send callback: tells you OK vs FAIL
static void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.printf(">>> ESPNOW Send to %02X:%02X:%02X:%02X:%02X:%02X : %s\n",
    mac_addr[0],mac_addr[1],mac_addr[2],
    mac_addr[3],mac_addr[4],mac_addr[5],
    status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL"
  );
}

void initEspNow() {
  // 1) ensure Wi-Fi STA mode and sync channel
  WiFi.mode(WIFI_STA);
  syncEspNowToWiFiChannel();

  // 2) init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error: ESP-NOW init failed");
    return;
  }

  // 3) register callbacks
  esp_now_register_recv_cb(onDataRecv);
  esp_now_register_send_cb(onDataSent);
}

void registerPeer(const uint8_t mac[6]) {
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, mac, 6);
  peer.ifidx    = WIFI_IF_STA;  // station interface
  peer.channel  = 0;            // 0 = use current channel
  peer.encrypt  = false;        // no encryption

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Error: failed to add peer");
  } else {
    Serial.printf("Added peer %02X:%02X:%02X:%02X:%02X:%02X\n",
      mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  }
}

esp_err_t sendPayload(const Payload* p) {
  const uint8_t* d = reinterpret_cast<const uint8_t*>(p);
  size_t len = sizeof(*p);

  // 1) dump payload
  Serial.print(">>> ESPNOW Payload (hex): ");
  for (size_t i = 0; i < len; i++) Serial.printf("%02X ", d[i]);
  Serial.println();
  Serial.print(">>> ESPNOW Payload (ascii): \"");
  Serial.write(d, len);
  Serial.println("\"");

  // 2) send to each peer
  esp_err_t result = ESP_OK;
  for (size_t i = 0; i < NUM_PEERS; ++i) {
    result = esp_now_send(PEER_MACS[i], d, len);
    if (result != ESP_OK) break;
  }
  return result;
}
