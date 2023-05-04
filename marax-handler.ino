#include <Arduino.h>
#include <Button2.h>



#include "pins.h"
#include "mara_timer.h"
#include "MaraDisplayer.h"
#include "MaraWifiServer.h"
#include "MaraSerialReader.h"
#include "secrets.h"
#include "debug.h"

Button2 buttonA = Button2(BUTTONA_PIN);
Button2 buttonB = Button2(BUTTONB_PIN);
MaraDisplayer* md;

MaraWifiServer* mws;
MaraSerialReader* msr;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting setup");


    // enable pin reading
    pinMode(PUMP_PIN, INPUT_PULLUP);
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
#ifdef DEBUG
    timer = millis();
#else
    timer = stopwatch();
#endif
    md->drawTimerArc(timer);
    delay(100);
}
