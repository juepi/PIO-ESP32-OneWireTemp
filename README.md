# Introduction 
A simple sketch to monitor OneWire temperature sensors using HTTP and MQTT.  
It is based on my [PIO-ESP32-Template](https://github.com/juepi/PIO-ESP32-Template), please consult the readme of the template on the prerequisites and how to compile the project.  
**Note:** I am not using DS18B20 sensor addresses, but the function `getTempCByIndex` to readout the connected DS18B20 sensors. To my knowledge, sensor order will not change during ESP reboots when using this function.

## MQTT Topics
In the `user-config.h` you can configure your topic tree (prepended to all used topics for OTA updating and also user-topics) and also the desired name of the OneWire topics (see `t_OW_T_Templ`). A sequential numbers (starting with `0`) will be automatically added to each connected sensor topic.

## Release History

### Initial commit (WiP)
- Basic functionality added
- Webservice and MQTT tested