
#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class GlobalState {
public:
    GlobalState();
    ~GlobalState();

    bool getWifiConnected();
    void setWifiConnected(bool connected);

    bool getMqttConnected();
    void setMqttConnected(bool connected);

private:
    struct State {
        bool wifi_connected;
        bool mqtt_connected;
    };

    State state;
    SemaphoreHandle_t mutex;
};

#endif // GLOBAL_STATE_H
