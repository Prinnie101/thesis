// creditModule.cpp

#include "creditModule.h"
#include <ArduinoJson.h>
#include <Preferences.h>
#include "config.h"    // for CREDIT_TOPIC, RELAY_PIN
#include <LiquidCrystal_I2C.h>

// ── Internal state ─────────────────────────────────────────
static float        receivedCredit   = 0.0f;
static int          lastBeepedCredit = -1;    // last integer credit we beeped
static unsigned long lastUpdate      = 0;     // for LCD refresh timing
static const int    buzzerPin        = 15;    // your buzzer pin
static Preferences  preferences;              // NVS handler

// ── Initialization ─────────────────────────────────────────
void creditInit(PubSubClient& client, LiquidCrystal_I2C& lcd) {
  // MQTT subscribe for credit updates
  client.subscribe(CREDIT_TOPIC, 1);

  // Buzzer setup
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // NVS init & load last saved credit
  preferences.begin("creditMod", false);
  receivedCredit = preferences.getFloat("credit", 0.0f);

  // LCD startup and initial display
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REMAINING ENERGY");
  lcd.setCursor(0, 1);
  char buf[16];
  dtostrf(receivedCredit, 6, 3, buf);
  lcd.print(buf);
}

// ── MQTT Callback ──────────────────────────────────────────
void creditCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, CREDIT_TOPIC) != 0) return;

  StaticJsonDocument<64> doc;
  if (deserializeJson(doc, payload, length)) return;
  receivedCredit = doc["kwh"].as<float>();

  // Persist the new credit value
  preferences.putFloat("credit", receivedCredit);

  // Beep when hitting integer thresholds 3,2,1
  int creditInt = (int)receivedCredit;
  if (creditInt != lastBeepedCredit) {
    if (creditInt == 3) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzerPin, HIGH); delay(200);
        digitalWrite(buzzerPin, LOW);  delay(200);
      }
    }
    else if (creditInt == 2) {
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzerPin, HIGH); delay(200);
        digitalWrite(buzzerPin, LOW);  delay(200);
      }
    }
    else if (creditInt == 1) {
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
    }
    lastBeepedCredit = creditInt;
  }

  // Turn relay off if credit is zero or below
  if (receivedCredit <= 0.0f) {
    digitalWrite(RELAY_PIN, HIGH);
  }
}

// ── LCD Refresh Loop ────────────────────────────────────────
void creditLoop(LiquidCrystal_I2C& lcd) {
  // throttle updates to every 100 ms
  if (millis() - lastUpdate < 100) return;
  lastUpdate = millis();

  // Clear second line
  lcd.setCursor(0, 1);
  for (uint8_t i = 0; i < 16; i++) lcd.print(' ');

  // Center and print credit value
  char buf[16];
  dtostrf(receivedCredit, 6, 3, buf);
  int len      = strlen(buf);
  int startCol = (16 - len) / 2;
  lcd.setCursor(startCol, 1);
  lcd.print(buf);
}

// ── ESP-NOW Dispatch ───────────────────────────────────────
void creditModuleUpdate(float credit, LiquidCrystal_I2C& lcd) {
  // Called when a credit update arrives over ESP-NOW
  receivedCredit = credit;
  // You can optionally persist and/or update the LCD here
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Credit:");
    lcd.setCursor(0, 1);
    lcd.print(receivedCredit, 2);
}

// ── Getter for main sketch ──────────────────────────────────
float getStoredCredit() {
  return receivedCredit;
}
