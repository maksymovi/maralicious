#include "MaraWifiServer.h"
#include "WiFi.h"

MaraWifiServer::MaraWifiServer(unsigned int port, const char *const ssid, const char *const password)
    : server(port)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int status = WiFi.waitForConnectResult();
    if (status == WL_CONNECTED)
    {
        Serial.print("Connected to wifi with IP ");
        Serial.println(WiFi.localIP());
        properlyInitialized = true;
        server.begin();
    }
    else
    {
        Serial.println("Failed to connect to WiFi");
        properlyInitialized = false;
    }
}

bool MaraWifiServer::broadcastMessage(const char *const message)
{
    if (!properlyInitialized)
    {
        return false;
    }

    

    WiFiClient client = server.available();
    if (client)
    {
        clients.push_back(client);
    }

    //theoretically I should be able to simplify all this down into server.write(message),
    //but the people at espressify apparently forgot to implement that function so lol
    for (auto it = clients.begin(); it != clients.end();)
    {
        if (it->connected())
        {
            it->print(message);
            ++it;
        }
        else
        {
            it = clients.erase(it);
        }
    }

    return true;
}