# ESP32-temp-humidity-sensor
ESP32-based temp and humidity sensor to be used with e.g Node-Red backend

## Features:
* Uses WiFi for internet access
* POSTs temp & humidity data to a HTTP endpoint 
* Optional deep sleep for low electricity usage 
* Optional serial debugging

## Required hardware:
* ESP32 based microcontroller (e.g ESP-WROOM-32)
* DHT11 temperature & humidity sensor
* 10kohm resistor

## Setup:
1. Connect DHT11 to ESP32 according to manual
2. Get needed dependencies from #include
3. Configure router settings and data pin from the code
4. Power up & flash by your preferred method
