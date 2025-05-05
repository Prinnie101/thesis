// sensorModule.h
#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <PubSubClient.h>

void sensorInit(PubSubClient& client);
void sensorLoop(PubSubClient& client);

#endif
