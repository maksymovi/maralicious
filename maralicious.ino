#include <Arduino.h>


#include "src/config/secrets.h"
#include "src/config/debug.h"
#include "src/config/pins.h"

#include "src/MaraDisplayer.h"
#include "src/MaraWifiServer.h"
#include "src/MaraSerialReader.h"
#include "src/MaraTimer.h"

MaraDisplayer* md;
MaraWifiServer* mws;
MaraSerialReader* msr;
MaraTimer* mt;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting setup");

    // timer
    mt = new MaraTimer(PUMP_PIN);
    // display 
    md = new MaraDisplayer();
    // wifi
    mws = new MaraWifiServer(80, wifiSsid, wifiPassword);
    //serial
    msr = MaraSerialReader::getInstance();
    Serial.println("Ending setup");
}

void loop()
{
    char buffer[128];
    MaraSerialReader::MaraXOutput maraXOutput;
    long timer;

    if(msr->readMaraSerial(maraXOutput))
    {
        md->drawTempArc(maraXOutput.hxTemp, maraXOutput.steamTemp);
        md->drawHeaterOnDot(maraXOutput.heaterOn);
        md->drawStatusChar(maraXOutput.steamOrBrew);
        sprintf(buffer, "Read: %s\n", maraXOutput.completeString.c_str());
        mws->broadcastMessage(buffer);
    }
    else
    {
        sprintf(buffer, "Failed to parse :%s\n", maraXOutput.completeString.c_str());
        mws->broadcastMessage(buffer);
    }
    timer = mt->stopwatch();

    md->drawTimerArc(timer);
    delay(100);
}
