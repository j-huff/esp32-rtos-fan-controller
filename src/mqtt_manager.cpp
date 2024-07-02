#include "mqtt_manager.h"
#include <Arduino.h>

MQTTManager::MQTTManager(PubSubClient& client, const char* server, int port,
                         const char* username, const char* password, GlobalState& state)
    : client(client), server(server), port(port), 
      username(username), password(password), globalState(state) {}

void MQTTManager::reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), username, password)) {
            Serial.println("connected");
            //client.subscribe("your/topic");  // Subscribe to desired topics

            globalState.setMqttConnected(true);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}
