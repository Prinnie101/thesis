//offlineCreditModule.h
#ifndef OFFLINE_CREDIT_MODULE_H
#define OFFLINE_CREDIT_MODULE_H

#include <LiquidCrystal_I2C.h>

/// Call once in setup() if you detect “no internet”
void offlineCreditModuleInit(LiquidCrystal_I2C& lcd);

/// Call in loop() when still offline
void offlineCreditModuleLoop(LiquidCrystal_I2C& lcd);

#endif // OFFLINE_CREDIT_MODULE_H
