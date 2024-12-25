/*
 * ESP32 Template
 * ==================
 * User specific function "user_loop" called in main loop
 * User specific funtion "user_setup" called in setup loop
 * Add stuff you want to run here
 */
#include "setup.h"

const char *index_html PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
    <title>OneWire Temperatures</title>
    <meta http-equiv="refresh" content="5" />
</head>
<body>
    <h3>System Status</h3>
<p>
<b>WiFi RSSI:</b> %TEMPL_WIFI_RSSI% dBm<br>
<b>MQTT:</b> %TEMPL_MQTT_STAT%<br>
<b>Uptime:</b> %TEMPL_UPTIME%<br>
</p>
    <h3>OneWire Sensors</h3>
<p>
<b>Status:</b>%TEMPL_OW_STAT%<br>
%TEMPL_OW%
</p>
    <h3>Control ESP</h3>
<p>
<form action="/get" method="get">
    <input type="submit" name="action" value="Reboot" />
</form>
</p> 
</body>
</html>
)";
const char *PARAM_MESSAGE PROGMEM = "action";

float OW_temperatures[MAX_OW_SENS] = {0.0F};
int OW_Status = 0;
const char *Stat_Decoder[] = {"Ok", "NoSensFound", "Read_Fail"};

AsyncWebServer server(80);
OneWire oneWire(PIN_OWDATA);
DallasTemperature OWtemp(&oneWire);

/*
 * User Setup function
 * ========================================================================
 */
void user_setup()
{
  // AsyncWebserver initialization
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html, processor); });
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
      if (message == "Reboot") {
        if (UptimeSeconds < 10)
        {
          request->redirect("/");
        }
        else
        {
          ESP.restart();
        }
      }
    } });
  server.onNotFound(notFound);
  server.begin();

  // OneWire initialization
  OWtemp.begin();
  OWtemp.setResolution(OWRES);
}

/*
 * User Main Loop
 * ========================================================================
 */
void user_loop()
{
  static uint32_t LastPub = 0;
  static uint32_t LastReadout = 0;
  float Temp = 0.0F;

  if ((JustBooted) || (UptimeSeconds - LastReadout) >= OW_READOUT_SEC)
  {
    if (OWtemp.getDeviceCount() > 0)
    {
      OWtemp.requestTemperatures(); // blocking call
      OW_Status = 0;                // assume correct readout
      for (int i = 0; i < (int)OWtemp.getDeviceCount(); i++)
      {
        Temp = OWtemp.getTempCByIndex(i);
        if (Temp == DEVICE_DISCONNECTED_C || Temp == DEVICE_FAULT_OPEN_C)
        {
          // readout failed for a sensor, abort this loop
          OW_Status = 2;
          break;
        }
        else
        {
          OW_temperatures[i] = Temp;
        }
      }
    }
    else
    {
      // no sensors found
      OW_Status = 1;
    }
    LastReadout = UptimeSeconds;
#ifdef ONBOARD_LED
    ToggleLed(LED, 1, 1);
#endif
  }

  // Publish stuff to MQTT broker
  if (((JustBooted) || (UptimeSeconds - LastPub) >= MQTT_PUB_SEC) && NetState == NET_UP)
  {
    mqttClt.publish(t_OW_Stat_T, String(Stat_Decoder[OW_Status]).c_str(), false);

    if (OW_Status == 0)
    {
      for (int i = 0; i < (int)OWtemp.getDeviceCount(); i++)
      {
        String MqttTopStr = String(t_OW_T_Templ) + i;
        mqttClt.publish(MqttTopStr.c_str(), String(OW_temperatures[i], 0).c_str(), false);
        yield();
      }
    }
    LastPub = UptimeSeconds;
  }
}

//
// 404
//
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

//
// Website processor to replace templates of our HTML page with useful data
//
String processor(const String &var)
{
  if (var == "TEMPL_WIFI_RSSI")
    return (String(WiFi.RSSI()));
  if (var == "TEMPL_UPTIME")
    return (String(UptimeSeconds));
  if (var == "TEMPL_MQTT_STAT")
  {
    if (mqttClt.connected())
    {
      return F("<font color=\"green\">connected</font>");
    }
    else
    {
      return F("<font color=\"red\">not connected!</font>");
    }
  }
  if (var == "TEMPL_OW_STAT")
    return String(Stat_Decoder[OW_Status]);
  if (var == "TEMPL_OW")
  {
    String ReturnString = "";
    if (OWtemp.getDeviceCount() > 0)
    {
      for (int i = 0; i < (int)OWtemp.getDeviceCount(); i++)
      {
        ReturnString = ReturnString + "<b>T" + i + ":</b> " + String(OW_temperatures[i], 0) + "<br>";
      }
      return ReturnString;
    }
    else
    {
      return F("<b><font color=\"red\">No Sensors found!</font></b>");
    }
  }
  return String();
}
