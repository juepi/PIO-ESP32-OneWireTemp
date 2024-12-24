# Introduction 
A simple sketch to monitor OneWire temperature sensors using HTTP and MQTT.  
It is based on my [PIO-ESP32-Template](https://github.com/juepi/PIO-ESP32-Template), please consult the readme of the template on the prerequisites and how to compile the project.  
**Note:** I am not using DS18B20 sensor addresses, but the function `getTempCByIndex` to readout the connected DS18B20 sensors. To my knowledge, sensor order will not change between ESP reboots when using this function.

## Sensor wiring
The sketch uses `IO5` as the OneWire data pin per default. Note that there might be problems with OneWire data pin at IO ports >36 on ESP32-S2 chips. I have used a 3k3 resistor as pullup, powering the sensors from the ESP32 3V3 pin, which seems to work fine with ~3m cable length and 3 sensors.

## MQTT Topics
In the `user-config.h` you can configure your topic tree (prepended to all used topics for OTA updating and also user-topics) and also the desired name of the OneWire topics (see `t_OW_T_Templ`). A sequential number (starting with `0`) will be automatically added to each connected sensor topic.  
OW communication status will be published to `t_OW_Stat_T`, all topics will be published in a configurable interval (`MQTT_PUB_SEC`).

## Webservice
The website shows WiFi connection quality, MQTT connection status as well as OneWire sensor data and state. The webpage will automatically refresh every 5 seconds. You can also reboot the ESP through the webinterface. The main purpose of the website is to check MQTT/sensor functionality and the sensor order.

## Release History

### Initial commit (WiP)
- Basic functionality added
- Webservice and MQTT tested

### v1.0.0
- Tested with 3 DS18B20 sensors
