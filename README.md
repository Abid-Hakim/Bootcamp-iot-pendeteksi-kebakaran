# Bootcamp-iot-pendeteksi-kebakaran
Sistem Peringatan Dini Kebakaran Berbasis IoT ini dirancang untuk mendeteksi potensi bahaya kebakaran secara real-time di dalam ruangan. Sistem menggunakan mikrokontroler ESP32 untuk membaca data dari sensor suhu (DHT22), sensor konsentrasi asap (MQ-2), dan sakelar simulasi deteksi api (Flame Sensor).

Data hasil pembacaan diproses secara lokal untuk mengendalikan indikator visual (LED Kuning & Merah), alarm suara (Buzzer Pasif), serta aktuator darurat (Relay pelepas pintu/sistem pemadam). Selain itu, sistem menampilkan informasi pada layar OLED SSD1306, meneruskan data telemetry menggunakan protokol MQTT ke broker HiveMQ, serta melakukan pembuatan log database otomatis ke file CSV melalui dashboard Node-RED.

| Nama Komponen | Komponen,Pin | Fungsi |
| :--- | :--- | :--- |
| **ESP32 DevKit v4** | Board Utama | Mikrokontroler pemroses data & komunikasi WiFi/MQTT |
| **DHT22** | Data -> GPIO 4 | Membaca data suhu lingkungan (°C) |
| **MQ-2 Gas Sensor** | AOUT -> GPIO 34 | Membaca konsentrasi asap/gas (PPM) |
| **Flame Button** | Signal -> GPIO 17 | Simulasi sensor api (`LOW` = Terdeteksi) |
| **OLED SSD1306** | SDA -> GPIO 21, SCL -> GPIO 22 | Tampilan status sistem dan data sensor lokal |
| **LED Kuning** | Anoda (+) -> GPIO 15 | Indikator visual status Peringatan (*Warning*) |
| **LED Merah** | Anoda (+) -> GPIO 16 | Indikator visual status Bahaya / Kritis (*Danger*) |
| **Buzzer Pasif** | Signal -> GPIO 18 | Alarm peringatan berupa suara melengking |
| **Relay Module** | IN -> GPIO 26 | Kontrol sakelar otomatis (Solenoid/Pintu Evakuasi) |

### Flowchart Sistem

```mermaid
flowchart TD
    A([Mulai / Booting ESP32]) --> B[Inisialisasi GPIO, DHT22, OLED, WiFi & MQTT]
    B --> C[Membaca Sensor: DHT22, MQ-2, Flame]
    C --> D[Konversi ADC MQ-2 ke PPM Asap]
    D --> E{Suhu >= 40°C AND\nAsap >= 300 PPM AND\nApi == Terdeteksi?}
    
    E -- Ya --> F["Status: KRITIS\n- LED Merah & Kuning: ON\n- Buzzer: ON (1000Hz)\n- Relay: ON"]
    E -- Tidak --> G{Suhu >= 40°C AND\nAsap >= 300 PPM?}
    G -- Ya --> H["Status: BAHAYA\n- LED Merah & Kuning: ON\n- Buzzer: ON (1000Hz)\n- Relay: OFF"]
    G -- Tidak --> I{Suhu >= 40°C OR\nAsap >= 300 PPM OR\nApi == Terdeteksi?}
    I -- Ya --> J["Status: PERINGATAN\n- LED Kuning: ON\n- Aktuator Lain: OFF"]
    I -- Tidak --> K["Status: AMAN\n- Semua Aktuator: OFF"]
    
    F --> L[Tampilkan Data pada Layar OLED]
    H --> L
    J --> L
    K --> L
    
    L --> M[Publish Payload JSON via MQTT]
    M --> N["Node-RED: Update UI & Append ke database_sensor.csv"]
    N --> O[Delay 2 Detik]
    O ([Selesai])
