// relayStateModule.h
#ifndef RELAY_STATE_MODULE_H
#define RELAY_STATE_MODULE_H

#include <PubSubClient.h>

// Call once in setup()
void relayStateInit(PubSubClient& client);

// Call on every loop()
void relayStateLoop(PubSubClient& client);

#endif
