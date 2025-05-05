// offlineCreditModule.cpp
#include "offlineCreditModule.h"
#include <Preferences.h>
#include <Arduino.h>

static Preferences preferences;
static unsigned long lastUpdate      = 0;
static int          lastBeepedCredit = -1;    // track last integer credit we beeped
static const int    buzzerPin        = 15;    // same pin as online

void offlineCreditModuleInit(LiquidCrystal_I2C& lcd) {
  // NVS load
  preferences.begin("creditMod", false);
  float stored = preferences.getFloat("credit", 0.0f);

  // Buzzer setup
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // ── SPLASH: “Switching to OFFLINE MODE” ──
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Switching to");
  lcd.setCursor(0, 1);
  lcd.print("OFFLINE MODE");
  delay(1500);

  // Initialize lastBeepedCredit so we don't beep immediately
  lastBeepedCredit = (int)stored;

  // ── Show persisted credit ──
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REMAINING ENERGY");
  lcd.setCursor(0, 1);
  char buf[16];
  dtostrf(stored, 6, 3, buf);
  lcd.print(buf);
}

void offlineCreditModuleLoop(LiquidCrystal_I2C& lcd) {
  // update only every 100 ms
  if (millis() - lastUpdate < 100) return;
  lastUpdate = millis();

  // reload stored credit
  float stored = preferences.getFloat("credit", 0.0f);
  int creditInt = (int)stored;

  // beep exactly as in online module when integer credit changes
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

  // update display
  char buf[16];
  dtostrf(stored, 6, 3, buf);

  lcd.setCursor(0, 1);
  for (uint8_t i = 0; i < 16; i++) lcd.print(' ');
  int pos = (16 - strlen(buf)) / 2;
  lcd.setCursor(pos, 1);
  lcd.print(buf);
}
