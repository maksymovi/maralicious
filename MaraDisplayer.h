#pragma once

#include <TFT_eSPI.h>
#include <SPI.h>
#include <User_Setup_Select.h>

/// @brief Displays stats from the MaraX on a TFT display
class MaraDisplayer
{
    public:
        MaraDisplayer();
        void drawSetup();
        void drawTimerArc(time_t currentTime);
        void drawTempArc(int hxTemp, int steamTemp);
        void drawHeaterOnDot(bool heaterOn);
        void drawStatusChar(char statusChar);
    private:
        void drawTimerArcOutline();
        void drawTempArcOutline();
        int arcDegreeFromChord(int radius, int chord);
        
        int timerRadius ;
        int timerCenterX;
        int timerCenterY;
        int barStart;
        int barEnd;
        int greenBarStart;
        int greenBarEnd;
        int yellowBarStart;
        int yellowBarEnd;

        int tempCenterX;
        int tempCenterY;
        int tempRadius;
        int targetTemperatureArcAngle;
        int targetTempStart;
        int targetTempEnd;
};