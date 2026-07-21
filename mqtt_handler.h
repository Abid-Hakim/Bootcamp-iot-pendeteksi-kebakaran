#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>
#include <WiFi.h>

void setup_wifi();
void mqtt_setup();
void mqtt_reconnect();
void mqtt_loop();
void mqtt_publish(String payload);
void mqtt_callback(char* topic, byte* payload, unsigned int length);

#endif
