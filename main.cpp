#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "config.h"
#include "mqtt_handler.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DHT dht(DHT_PIN, DHT_TYPE);


int baseline_gas = 0;
void setup() {
  Serial.begin(115200);
  
  // Inisialisasi GPIO
  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Kondisi awal mati semua
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  noTone(BUZZER_PIN);
  digitalWrite(RELAY_PIN, LOW);

  dht.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Memulai Sistem...");
  display.display();
  
  setup_wifi();
  mqtt_setup();

  baseline_gas = analogRead(MQ2_PIN);
  
  Serial.println("Sistem Siap!");
}

void loop() {
  mqtt_loop();
  
  float temp = dht.readTemperature();
  int flame_status = digitalRead(FLAME_PIN); 
  // ...
  int gas_analog = analogRead(MQ2_PIN);
  
  // Kurangi pembacaan dengan nilai awal (baseline)
  int selisih_gas = gas_analog - baseline_gas;
  
  // Cegah nilai menjadi negatif jika slider digeser lebih rendah dari titik awal
  if (selisih_gas < 0) selisih_gas = 0; 
  
  // Map nilai selisihnya ke PPM
  int gas_ppm = map(selisih_gas, 0, (4095 - baseline_gas), 0, 10000);
  
  // ... (lanjutkan kode Anda)
  
  // Karena sekarang nilainya bisa sampai 10000, sesuaikan ambang batasnya
  int BATAS_ASAP = 300; 

  if (isnan(temp)) temp = 0;

  // Variabel state aktuator
  bool led_red = LOW;
  bool led_yellow = LOW;
  bool buzzer = LOW;
  bool relay = LOW;

  // LOGIKA SKENARIO ATURAN ALARM
  if (temp >= 40 && gas_ppm >= BATAS_ASAP && flame_status == LOW) {
    buzzer = HIGH; led_red = HIGH; led_yellow = HIGH; relay = HIGH;
  } 
  else if (temp >= 40 && gas_ppm >= BATAS_ASAP) {
    led_red = HIGH; led_yellow = HIGH;
  } 
  else if (temp >= 40 && flame_status == LOW) {
    led_red = HIGH; led_yellow = HIGH;
  }
  else if (gas_ppm >= BATAS_ASAP && flame_status == LOW) {
    led_red = HIGH; led_yellow = HIGH;
  }
  else if (temp >= 40 || gas_ppm >= BATAS_ASAP || flame_status == LOW) {
    led_yellow = HIGH;
  }

  // Terapkan state ke aktuator
  digitalWrite(LED_RED_PIN, led_red);
  digitalWrite(LED_YELLOW_PIN, led_yellow);
  digitalWrite(RELAY_PIN, relay);

  if (buzzer == HIGH) {
    tone(BUZZER_PIN, 1000); 
  } else {
    noTone(BUZZER_PIN);
  }

  // UPDATE LAYAR OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.printf("Suhu : %.1f C\n", temp);
  
  // Nilai %d sekarang menampilkan hasil konversi (PPM sesungguhnya)
  display.printf("Asap : %d ppm\n", gas_ppm); 
  
  display.printf("Api  : %s\n", (flame_status == LOW) ? "TERDETEKSI" : "AMAN");
  display.printf("Status: %s\n", (led_red && relay) ? "KRITIS" : (led_red ? "BAHAYA" : (led_yellow ? "PERINGATAN" : "AMAN")));
  
  display.display();

  // PUBLISH KE MQTT
  String payload = "{\"suhu\": " + String(temp) + 
                   ", \"asap\": " + String(gas_ppm) + 
                   ", \"api\": " + String(flame_status == LOW ? 1 : 0) + 
                   ", \"led_merah\": " + String(led_red) + 
                   ", \"led_kuning\": " + String(led_yellow) + 
                   ", \"buzzer\": " + String(buzzer) + 
                   ", \"relay\": " + String(relay) + "}";
  mqtt_publish(payload);

  delay(2000);
}
