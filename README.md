# UnifiedESP

> Keep your ESP32 energy-management game on fleek ⚡️

**UnifiedESP** is a modular Arduino/ESP32 sketch that vibes with sensor data, credit tracking, and relay control—both online (MQTT) and offline (ESP-NOW). No cap, it’s perfect for boarding-house setups, maker projects, or any situation where you need to monitor power usage, deduct “credits,” and switch relays without breaking a sweat.

## 🚀 Features

- **(Wi-Fi + MQTT):**  
  - Publishes voltage, current & energy readings (via PZEM-004T v3).  
  - Listens for relay commands over MQTT.
- **Credit Management:**  
  - Deducts credit based on power consumption.  
  - Shows balances on an I²C LCD (LiquidCrystal_I2C).  
- **Modular Codebase:**  
  - `wifi_module` – Wi-Fi & MQTT connect + lifecycl
  - `sensorModule` – PZEM readings + JSON payloads  
  - `creditModule` – online credit logic + LCD display  
  - `relayModule` – MQTT-driven relay control  
  - `offlineCreditModule` & `offlineRelayModule` – offline fallbacks  
  - `relayStateModule` – track & sync relay status
  - 
## 🔧 Requirements

- **Hardware**  
  - ESP32 development board  
  - PZEM-004T v3.0 (voltage/current/energy sensor)  
  - Relay module (connected to `RELAY_PIN`)  
  - I²C LCD display (e.g. 16×2)  
- **Software**  
  - Arduino IDE (v1.8.13+) or PlatformIO  
  - ESP32 board support  
- **Libraries** (Install via Library Manager)  
  - `WiFi` (built-in ESP32 core)  
  - `PubSubClient`  
  - `ArduinoJson`  
  - `LiquidCrystal_I2C`  
  - `PZEM004Tv30`

---

## 🛠️ Installation & Setup

1. **Clone this repo**  
   git clone https://github.com/Prinnie101/thesis.git

Open in Arduino IDE

File → Open → UnifiedESP.ino

Install required libraries

Sketch → Include Library → Manage Libraries…

Search for & install: PubSubClient, ArduinoJson, LiquidCrystal_I2C, PZEM004Tv30

Edit config.h

static const char* SSID         = "your_wifi_ssid";
static const char* WIFI_PASS    = "your_wifi_pass";
static const char* MQTT_SERVER  = "mqtt.example.com";
static const int   MQTT_PORT    = 1883;

// MQTT topics
#define SENSOR_TOPIC      "boardinghouse/sensor/data"
#define RELAY_TOPIC       "boardinghouse/relay/command"
#define CREDIT_TOPIC      "boardinghouse/credit/1"
#define RELAY_STATE_TOPIC "boardinghouse/relay/state"

// Pin definitions
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define RELAY_PIN   18

// ESP-NOW peers
static const size_t NUM_PEERS = 1;
static const uint8_t PEER_MACS[NUM_PEERS][6] = {
  { 0x24, 0x6F, 0x28, 0xBD, 0x47, 0x68 }
};

Select board & port

Tools → Board → “ESP32 Dev Module” (or your variant)

Tools → Port → COMx (or /dev/ttyUSBx)

Upload & enjoy

Click ➤ Upload

Open Serial Monitor for debug logs at 115200 baud

📂 Project Structure

UnifiedESP/
├── UnifiedESP.ino           # Main sketch  
├── config.h                 # Wi-Fi, MQTT & pin configs  
├── INCLUDEALLUnifiedESP.h   # Convenience header for all modules  
├── wifi_module.{h,cpp}      # Wi-Fi & MQTT setup + loop  
├── espnow_module.{h,cpp}    # ESP-NOW init, send/receive  
├── sensorModule.{h,cpp}     # PZEM readings + JSON + publish  
├── creditModule.{h,cpp}     # Online credit mgmt + LCD  
├── relayModule.{h,cpp}      # Relay on/off via MQTT  
├── offlineCreditModule.{h,cpp}  # Offline credit + LCD  
├── offlineRelayModule.{h,cpp}   # Offline relay control  
├── relayStateModule.{h,cpp}     # Sync & track relay state  
└── README.md                # ← You are here  

🤝 Contributing
PRs & issues welcome! Feel free to tweak modules, add features, or drop bug reports. Let’s make power-management more lit together 🔥

