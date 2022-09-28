#include <ESP8266WiFi.h>

struct WifiSetup
{

  void Start(const char * WIFI_SSID, const char * WIFI_PASS)
  {
    WiFi.mode(WIFI_STA);

    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
       Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
      delay(1000);
    }
    Serial.println();

    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  }
};
