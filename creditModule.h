// creditModule.h

#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>

// Initialize the credit module: subscribe to MQTT and set up LCD
void creditInit(PubSubClient& client, LiquidCrystal_I2C& lcd);

// MQTT callback for incoming credit messages
void creditCallback(char* topic, byte* payload, unsigned int length);

// Periodic LCD update
void creditLoop(LiquidCrystal_I2C& lcd);

// Update the stored credit (used by ESP-NOW receive path)
// void creditModuleUpdate(float credit);
void creditModuleUpdate(float credit, LiquidCrystal_I2C& lcd);

// Returns the most recently stored credit balance
float getStoredCredit();
