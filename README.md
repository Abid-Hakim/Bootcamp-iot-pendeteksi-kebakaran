# Bootcamp-iot-pendeteksi-kebakaran
Sistem Peringatan Dini Kebakaran Berbasis IoT ini dirancang untuk mendeteksi potensi bahaya kebakaran secara real-time di dalam ruangan. Sistem menggunakan mikrokontroler ESP32 untuk membaca data dari sensor suhu (DHT22), sensor konsentrasi asap (MQ-2), dan sakelar simulasi deteksi api (Flame Sensor).

ARSITEKTUR :
[ LAYER PERSEPSI ]             [ LAYER JARINGAN ]            [ LAYER APLIKASI ]
+--------------------+         +--------------------+        +--------------------+
| - DHT22 (Suhu)     |         |                    |        |  Node-RED Dashboard|
| - MQ-2 (Asap)      | WiFi    |   MQTT Broker      | MQTT   |  - Visualisasi UI  |
| - Flame Button     |-------> | (broker.hivemq.com)|------->|  - Local CSV DB    |
| - ESP32 Micro      | TCP/IP  |   Port: 1883       |        |  - Remote Control  |
| - OLED, LED, Relay |         |                    |        |                    |
+--------------------+         +--------------------+        +--------------------+

Komponen dan PIN :
Nama Komponen,Pin Komponen,Pin ESP32 / Modul,Mode / Protokol,Fungsi / Keterangan
DHT22,SDA / Data,GPIO 4,Digital Input,Membaca Suhu & Kelembapan
MQ-2 Gas Sensor,AOUT,GPIO 34,Analog Input (ADC1),Membaca Konsentrasi Asap/Gas
Flame Button,1.L (Signal),GPIO 17,Digital Input (PULLUP),Simulasi Api (LOW = Terdeteksi)
OLED SSD1306,SDA,GPIO 21,I2C Data,Layar Tampilan Status Lokal
OLED SSD1306,SCL,GPIO 22,I2C Clock,Clock Komunikasi I2C
LED Kuning,Anoda (+),GPIO 15,Digital Output,Indikator Warning / Peringatan
LED Merah,Anoda (+),GPIO 16,Digital Output,Indikator Bahaya / Kritis
Buzzer Pasif,Pin 2 (Signal),GPIO 18,Digital Output (tone()),Alarm Suara (Frekuensi 1000 Hz)
Relay Module,IN,GPIO 26,Digital Output,Kontrol Pintu Evakuasi / Solenoid

Alur Demo :
                             +-----------------------+
                             |   Mulai Simulasi      |
                             +-----------------------+
                                         |
                                         v
                             +-----------------------+
                             | Evaluasi Skenario     |
                             +-----------------------+
                                         |
        +-------------------+------------+------------+-------------------+
        |                   |                         |                   |
        v                   v                         v                   v
+---------------+   +---------------+         +---------------+   +---------------+
| Skenario 1:   |   | Skenario 2:   |         | Skenario 3:   |   | Skenario 4:   |
| Normal (Aman) |   | Peringatan    |         | Bahaya        |   | Kritis        |
+---------------+   +---------------+         +---------------+   +---------------+
