#include <Button2.h>
#include <TFT_eSPI.h>
#include <User_Setup_Select.h>

#include "pins.h"
#include "mara_serial.h"
#include "mara_timer.h"

Button2 buttonA = Button2(BUTTONA_PIN);
Button2 buttonB = Button2(BUTTONB_PIN);

TFT_eSPI tft = TFT_eSPI();

void setup()
{

    Serial.begin(115200);

    // display
    tft.init();
    tft.setRotation(2);
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);

    // enable pin reading
    pinMode(PUMP_PIN, INPUT_PULLUP);

    // enable MaraX debug serial reading
    Serial2.begin(9600, SERIAL_8N1, MARAX_RX_PIN, MARAX_TX_PIN);
}

char *str = "";
MaraXOutput maraXOutput;

void loop()
{
    char buff[100];
    long timer;

    tft.fillScreen(TFT_BLACK);
    tft.drawString(isTimePinEnabled ? "on" : "off", 10, 10);
    if (readMaraSerial(maraXOutput))
        str = maraXOutput.completeString;
    tft.drawString(str, 10, 30);
    timer = stopwatch();
    sprintf(buff, "%d.%d", timer / 1000, (timer % 1000) / 100);
    tft.drawString(buff, 10, 70, 6);
    delay(100);
}
