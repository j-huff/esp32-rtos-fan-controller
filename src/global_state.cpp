#include "global_state.h"

GlobalState::GlobalState() : state{false, false} {
    mutex = xSemaphoreCreateMutex();
}

GlobalState::~GlobalState() {
    vSemaphoreDelete(mutex);
}

bool GlobalState::getWifiConnected() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    bool connected = state.wifi_connected;
    xSemaphoreGive(mutex);
    return connected;
}

void GlobalState::setWifiConnected(bool connected) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    state.wifi_connected = connected;
    xSemaphoreGive(mutex);
}

bool GlobalState::getMqttConnected() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    bool connected = state.mqtt_connected;
    xSemaphoreGive(mutex);
    return connected;
}

void GlobalState::setMqttConnected(bool connected) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    state.mqtt_connected = connected;
    xSemaphoreGive(mutex);
}
