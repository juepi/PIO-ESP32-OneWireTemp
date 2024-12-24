/*
 *   OneWire Temperature sensor monitor based on
 *   ESP32 Template
 *   User specific configuration, defines and Function declarations
 */
#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "mqtt-ota-config.h"

// Define required user libraries here
// Don't forget to add them into platformio.ini
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Declare user setup and main loop functions
extern void user_setup();
extern void user_loop();
// Webserver functions
extern String processor(const String &var);
extern void notFound(AsyncWebServerRequest *request);

// MQTT Topic Tree prepended to all topics
// ATTN: Must end with "/"!
#define TOPTREE "HB7/UG/Heating/Sensors/"

// OneWire Settings
#define PIN_OWDATA 5                       // GPIO for OneWire communication (Note: high GPIOs >36 may not work!)
#define OWRES 9                            // Use 9 bits resolution (0.5°C)
#define MAX_OW_SENS 5                      // max amount of DS18B20 sensors connected
extern float OW_temperatures[MAX_OW_SENS]; // array for sensor temperatures
extern int OW_Status;                      // OneWire status
extern const char *Stat_Decoder[];           // helper array for OW status text messages
#define t_OW_T_Templ TOPTREE "OW_T"        // topic template for seonsors; will be extended with sensor numbers starting with 0
#define t_OW_Stat_T TOPTREE "OW_StatTXT"   // OneWire Status Text topic

// Frequency for readout and MQTT publishing
#define OW_READOUT_SEC 5
#define MQTT_PUB_SEC 60

// webserver stuff
extern const char *PARAM_MESSAGE;
extern const char *index_html;

#endif // USER_CONFIG_H