#include <Button2.h>
#include <TFT_eSPI.h>

#include "pins.h"

Button2 buttonA = Button2(BUTTONA_PIN);
Button2 buttonB = Button2(BUTTONB_PIN);

TFT_eSPI tft = TFT_eSPI();


#define SCREENYRES 240
#define SCREENXRES 135
#define XRES 135
#define YRES 120

void setup()
{

    Serial.begin(115200);

    // display
    tft.init();
    tft.setRotation(0);
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);

    // enable pin reading
    pinMode(PUMP_PIN, INPUT_PULLDOWN);

    // enable MaraX debug serial reading
    Serial2.begin(9600, SERIAL_8N1, MARAX_RX_PIN, MARAX_TX_PIN);
}

int i = 0;

void loop()
{
    String str = "";
    char buff[100];
    tft.fillScreen(TFT_BLACK);
    tft.drawString(digitalRead(PUMP_PIN) ? "on" : "off", 10, 10);
    if(Serial2.available())
    {
        str = Serial2.readString();
    }
    tft.drawString(str, 10, 20);
    sprintf(buff, "%d", i++);
    tft.drawString(buff, 10, 30);
    delay(100);
}