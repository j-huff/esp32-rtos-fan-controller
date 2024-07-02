#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "global_state.h"

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password, GlobalState& state);
    void setupWiFi();

private:
    const char* ssid;
    const char* password;
    GlobalState& globalState;
};

#endif // WIFI_MANAGER_H
