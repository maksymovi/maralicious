#include <Arduino.h>
#include <Button2.h>



#include "pins.h"
#include "mara_serial.h"
#include "mara_timer.h"
#include "mara_draw.h"

Button2 buttonA = Button2(BUTTONA_PIN);
Button2 buttonB = Button2(BUTTONB_PIN);

void setup()
{

    Serial.begin(115200);
    Serial.println("Starting setup");

    // display

    drawSetup();

    // enable pin reading
    pinMode(PUMP_PIN, INPUT_PULLUP);

    // enable MaraX debug serial reading
    Serial2.begin(9600, SERIAL_8N1, MARAX_RX_PIN, MARAX_TX_PIN);
    //delay(2000); //wait for marax to boot
    drawTimerArcOutline();
    Serial.println("Ending setup");

}

char *str = "";
MaraXOutput maraXOutput;

void loop()
{
    char buff[100];
    long timer;

    //tft.fillScreen(TFT_BLACK);
    //tft.drawString(isTimePinEnabled() ? "on" : "off", 10, 10);
    if (readMaraSerial(maraXOutput))
        str = maraXOutput.completeString;
    //tft.drawString(str, 10, 30);
    timer = stopwatch();
    //tft.drawString(buff, 10, 70, 6);
    
    drawTimerArc(millis());
    drawTempArc(92, 168);

    delay(100);
}
