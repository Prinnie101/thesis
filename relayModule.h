// relayModule.h
#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include <PubSubClient.h>

void relayInit(PubSubClient& client);
void relayCallback(char* topic, byte* payload, unsigned int length);
// Called by ESP-NOW when a relay-command packet arrives
void relayModuleCommand(bool on);

#endif
