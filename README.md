# Terrarium-Monitor-SHT40

A ESP32 terrarium monitor written in C++

## Features
* SHT40 sensor for temperature and humidity readings in the terrarium
* BME280 sensor for temperature, humidity, pressure and altitude readings of the ambient environment
* 0.96' SSD1306 oled display for viewing the live stats
* 6 pushbuttons to control the user interface

## Components Used
1. ESP32-S3 DevKit - 1
2. SHT40 Temperature and Humidity Sensor Probe with 1M cable - 1
3. BME280 Temperature, Humidity and Barometer Sensor Module - 1
4. 0.96' SSD1306 Oled Display - 1
5. 18650 Battery Holder - 2
6. MH CD42 LiPo Charger 3.7V 2A IN 5V + LED Level Indicator - 1
7. Basic Switch - 1
8. Tactile Pushbuttons - 7
9. Type C Breakout Board - 1
10. Female Header-Pins
11. Perf Board - 2

As the name suggests, this project is to create a portable monitoring device capable of measuring critical parameters within a terrarium (e.g. humidity, temperature) and reptile enclosures based on a SHT40 sensor. An external BME280 sensor module is also added to measure the ambient humidity, temperature, pressure and altitude and can be omitted if desired. 

## Why use the SHT40?

Compared to other common humidity sensors like the DHT22, the SHT40 sensor module offers a significant upgrade in terms of performance and accuracy. 

| Sensor | SHT40  | DHT22 |
|------------- | ------------- |:-------------:|
| Communication Protocol | I2C | Proprietary Single-Bus |
| Avg. Current Consumption  (Standby) | 80nA | 60uA |
| Avg. Current Consumption (Measuring) | 0.4uA | 0.3mA |
| Resolution (Relative Humidity) | 0.01% | 0.1% |
| Resolution (Temperature) | 0.01C | 0.1C |
| Accuracy (Relative Humidity)| 1.8%  | 2.0% |
| Accuracy (Temperature) | 0.2C | 0.5C |

<img width="788" height="795" alt="image" src="https://github.com/user-attachments/assets/7e391d55-b9c1-4cdb-8a48-bc9af9e0c3a7" />

Other than specification upgrades, the SHT40 sensor also comes in a cabled probe version, making it more suitable for terrarium applications, where placing the entire device into the terrarium would be unrealistic. The cabled verison also comes in a stainless steel metal casing, offering some protection against water splashes. 

The sensor also contains heating functionality, allowing the sensor to clear up condensation on the sensor surface. Since the humidity within the terrarium is relatively high, this is quite important as condensation on the sensor surface can give false humidity readings.

The user can also change device configurations on the device itself using the designed settings page. 
It allows the user to change various parameters such as display refresh rate, device sleep timeout, sensor heating and etc easily without having to reprogram the device entirely. 

## Timeline
The device currently has the following features:
* Humidity, temperature readings using both BME280 and SHT40
* Pressure and estimated altitude readings using BME280
* Fully functional settings page to change various device configurations
* Display blanks and sensor reading halted when reaching sleep timeout to reduce power consumption when not in use

Features to add in the future:
* Sensor heating capability (currently not available yet since my SHT40 broke T-T)
* Deep-Sleep Functionality (minimize power consumption to as low as possible)
