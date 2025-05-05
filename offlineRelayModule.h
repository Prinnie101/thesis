#ifndef OFFLINE_RELAY_MODULE_H
#define OFFLINE_RELAY_MODULE_H

/// Call once in setup() if you detect “no internet”
void offlineRelayModuleInit();

/// Call in loop() when still offline
void offlineRelayModuleLoop();

#endif // OFFLINE_RELAY_MODULE_H
