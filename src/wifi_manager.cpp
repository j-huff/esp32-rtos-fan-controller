#include "wifi_manager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* password, GlobalState& state)
    : ssid(ssid), password(password), globalState(state) {}

void WiFiManager::setupWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    globalState.setWifiConnected(true);
}
