/*
  SphynxWiFi library: https://github.com/Snootic/sphynx_network_tools/tree/main/wifi-scan
  For Sphynx: https://github.com/PedroVidalDev/esp32-sphynx
  code to create AP mode for ESP32, host a web server and scan local wifi in the area.
  Connect to the web page on 192.168.4.1 or sphynx-dev.local and connect to a WiFi.
  By: Snootic & PedroVidalDev - 2024
*/

#ifndef SphynxWiFi_h
#define SphynxWiFi_h

#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include <Preferences.h>

class SphynxWiFiClass
{
    public:
        SphynxWiFiClass();
        const char* ssidAP = "Sphynx-WIFI";
        const char* senhaAP = "12345678";

        void setupWiFi();
        void scan();
        int connect();
        int status();
        void saveCredentials();
    private:
        //   const char* hostname = "sphynx-dev";
};
extern SphynxWiFiClass SphynxWiFi;
#endif