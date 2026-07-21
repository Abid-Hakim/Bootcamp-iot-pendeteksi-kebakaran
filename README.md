# Bootcamp-iot-pendeteksi-kebakaran
Sistem Peringatan Dini Kebakaran Berbasis IoT ini dirancang untuk mendeteksi potensi bahaya kebakaran secara real-time di dalam ruangan. Sistem menggunakan mikrokontroler ESP32 untuk membaca data dari sensor suhu (DHT22), sensor konsentrasi asap (MQ-2), dan sakelar simulasi deteksi api (Flame Sensor).

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
