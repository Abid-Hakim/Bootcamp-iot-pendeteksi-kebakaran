#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// PIN GPIO sesuai diagram.json
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define MQ2_PIN 34
#define FLAME_PIN 17
#define LED_RED_PIN 16
#define LED_YELLOW_PIN 15
#define BUZZER_PIN 18
#define RELAY_PIN 26

// Konfigurasi WiFi
extern const char* ssid;
extern const char* password;

// Konfigurasi MQTT
extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* mqtt_topic_pub;
extern const char* mqtt_topic_sub;

#endif
