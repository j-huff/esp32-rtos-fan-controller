

#include <Arduino.h>

#include "config.h"
#include "OTA.h"
#include "log.h"

#if defined(useOTAUpdate)
  // https://github.com/SensorsIot/ESP32-OTA
  #include "OTA.h"
  #if !defined(useOTA_RTOS)
    #include <ArduinoOTA.h>
  #endif
#endif
#if defined(useTelnetStream)
#include <TelnetStream.h>
#endif

#include "HAMqttDevice.h"
#include "global_state.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"

HAMqttDevice fanDevice("ESP-32 Fan", HAMqttDevice::FAN);


// WiFi credentials
const char* ssid = SECRET_WIFI_SSID;
const char* password = SECRET_WIFI_PASSWORD;

// MQTT Broker settings
const char* mqtt_server = SECRET_MQTT_BROKER_ADDRESS;
const int mqtt_port = SECRET_MQTT_PORT;
const char* mqtt_username = SECRET_MQTT_USERNAME;
const char* mqtt_password = SECRET_MQTT_PASSWORD;

// Global objects
WiFiClient espClient;
PubSubClient client(espClient);
GlobalState globalState;

WiFiManager wifiManager(ssid, password, globalState);
MQTTManager mqttManager(client, mqtt_server, mqtt_port, mqtt_username, mqtt_password, globalState);

void wifiTask(void* parameter) {
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            wifiManager.setupWiFi();
        }
        vTaskDelay(10000 / portTICK_PERIOD_MS);  // Check every 10 seconds
    }
}

void mqttTask(void* parameter) {
    while (true) {
        if (!client.connected()) {
            mqttManager.reconnectMQTT();
        }
        client.loop();
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Run frequently to handle MQTT messages
    }
}


void setup(){
  Serial.begin(115200);
  Serial.println("");
  Log.printf("Setup ...\r\n");

  #if defined(useOTAUpdate)
  OTA_setup("myesp32fan");
  // Comment line below to not start OTA. Save heap space and start it via MQTT only when needed.
  // ArduinoOTA.begin();
  #endif
  #if defined(useTelnetStream)
  TelnetStream.begin();
  #endif

  // Start WiFi connection task
  xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 1, NULL);
  // Start MQTT connection task
  xTaskCreate(mqttTask, "MQTTTask", 4096, NULL, 1, NULL);

  Log.printf("Setup Complete.\r\n");
}

void loop(){
  #if defined(useOTAUpdate) && !defined(useOTA_RTOS)
  // If you do not use FreeRTOS, you have to regulary call the handle method
  ArduinoOTA.handle();
  #endif

}
