#pragma once

#include "WiFi.h"

#include <list>

class MaraWifiServer
{
public:
    MaraWifiServer(unsigned int port, const char *const ssid, const char *const password);
    bool broadcastMessage(const char *const message);

    private:
        WiFiServer server;
        std::list<WiFiClient> clients;
        bool properlyInitialized;
};