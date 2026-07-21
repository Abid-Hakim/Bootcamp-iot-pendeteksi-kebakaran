#include "config.h"

// Konstanta WiFi Wokwi Default
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Konstanta Broker MQTT (Menggunakan public broker)
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic_pub = "iot/pendeteksi_api/status";
const char* mqtt_topic_sub = "iot/pendeteksi_api/kontrol";
