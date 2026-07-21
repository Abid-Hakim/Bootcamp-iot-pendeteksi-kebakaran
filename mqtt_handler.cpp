#include "mqtt_handler.h"
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("MQTT Received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
  
  // Kontrol manual tambahan misal mengendalikan pintu via relay
  if (String(topic) == mqtt_topic_sub) {
    if (message == "BUKA_PINTU") digitalWrite(RELAY_PIN, HIGH);
    else if (message == "TUTUP_PINTU") digitalWrite(RELAY_PIN, LOW);
  }
}

void mqtt_setup() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
}

void mqtt_reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_loop() {
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();
}

void mqtt_publish(String payload) {
  if (client.connected()) {
    client.publish(mqtt_topic_pub, payload.c_str());
  }
}
