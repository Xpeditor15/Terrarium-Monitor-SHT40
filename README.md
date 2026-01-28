# Terrarium-Monitor-SHT40

A ESP32 terrarium monitor written in C++

## Features
* SHT40 sensor for temperature and humidity readings in the terrarium
* BME280 sensor for temperature, humidity, pressure and altitude readings of the ambient environment
* 0.96' SSD1306 oled display for viewing the live stats
* 6 pushbuttons to control the user interface

## Components Used
1. ESP32-S3 DevKit - 1
2. SHT40 Temperature and Humidity Sensor with 1M cable - 1
3. BME280 Temperature, Humidity and Barometer Sensor Module - 1
4. 0.96' SSD1306 Oled Display - 1
5. 18650 Battery Holder - 2
6. MH CD42 LiPo Charger 3.7V 2A IN 5V + LED Level Indicator - 1
7. Basic Switch - 1
8. Tactile Pushbuttons - 7
9. 
10. Female Header-Pins
11. Perf Board - 2


As the name suggests, the project is to create a portable monitoring device capable of recording the humidity and temperature within a terrarium and reptile enclosures based on a SHT40 sensor. The system also has an external BME280 sensor to measure the ambient humidity, temperature, pressure and altitude. 

## Why use the SHT40?

Compared to other common humidity sensors like the DHT22, the SHT40 sensor module offers a significant upgrade in terms of performance and accuracy. 

| Sensor | SHT40  | DHT22 |
|------------- | ------------- |:-------------:|
| Communication Protocol      | I2C     |     Proprietary Single-Bus
| Avg. Current Consumption  (Standby)     |  80nA    | 60uA |
| Avg. Current Consumption (Measuring) | 0.4uA | 0.3mA |
| Resolution (Relative Humidity) | 0.01% | 0.1% |
| Resolution (Temperature) | 0.01C | 0.1C |
|   Accuracy (Relative Humidity)    |  1.8%  | 2.0% |
| Accuracy (Temperature) | 0.2C | 0.5C |

```mermaid
flowchart TD
    A[Christmas] -->|Get money| B(Go shopping);
    B --> C{Let me think};
    C -->|One| D[Laptop];
    C -->|Two| E[iPhone];
    C -->|Three| F[fa:fa-car Car];
```

