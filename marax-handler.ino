#include <Arduino.h>
#include <Button2.h>



#include "pins.h"
#include "mara_serial.h"
#include "mara_timer.h"
#include "MaraDisplayer.h"
#include "MaraWifiServer.h"
#include "secrets.h"

//comment for prod
#define DEBUG

Button2 buttonA = Button2(BUTTONA_PIN);
Button2 buttonB = Button2(BUTTONB_PIN);
MaraDisplayer* md;

MaraWifiServer* mws;

void setup()
{

    Serial.begin(115200);
    Serial.println("Starting setup");

    // display 

    md = new MaraDisplayer();

    // enable pin reading
    pinMode(PUMP_PIN, INPUT_PULLUP);

    // enable MaraX debug serial reading
    Serial2.begin(9600, SERIAL_8N1, MARAX_RX_PIN, MARAX_TX_PIN);
    //delay(2000); //wait for marax to boot

    // wifi
    mws = new MaraWifiServer(80, wifiSsid, wifiPassword);
    Serial.println("Ending setup");
}

//if debug, initialize maraXOutput with dummy values
//else, read from serial
#ifdef DEBUG
// C1.19,116,124,095,0560,1
char* maraDebugXOutput = "C1.19,116,124,095,0560,1";
#endif

void loop()
{
    mws->broadcastMessage("hello world");
    MaraXOutput maraXOutput;
    long timer;

#ifdef DEBUG
    if(parseMaraSerial(maraXOutput, maraDebugXOutput))
#else
    if (readMaraSerial(maraXOutput))
#endif
    {
        md->drawTempArc(maraXOutput.hxTemp, maraXOutput.steamTemp);
        md->drawHeaterOnDot(maraXOutput.heaterOn);
        md->drawStatusChar(maraXOutput.steamOrBrew);
    }
#ifdef DEBUG
    timer = millis();
#else
    timer = stopwatch();
#endif
    md->drawTimerArc(timer);
    delay(100);
}
