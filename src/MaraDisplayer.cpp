#include "MaraDisplayer.h"
#include <math.h>

#include "config/espresso_parameters.h"

#define ARC_ANGLE 260
#define ARC_WIDTH 12
#define TIMER_FONT 1
#define BACKGROUND_COLOR TFT_BLACK
#define TIMER_COLOR TFT_MAGENTA
#define TARGET_TIME_COLOR TFT_DARKGREEN
#define LESSER_TARGET_TIME_COLOR TFT_GREENYELLOW
#define HEATER_DOT_SIZE 5

//apparently this needs to be defined in global scope only
TFT_eSPI tft = TFT_eSPI();

MaraDisplayer::MaraDisplayer()
{
    tft.init();
    tft.invertDisplay(1);
    tft.setRotation(2);
    tft.setTextSize(3);
    tft.fillScreen(TFT_WHITE); //quick and dirty ghosting fix on boot
    delay(50);
    tft.fillScreen(BACKGROUND_COLOR);
    tft.setTextColor(TFT_DARKGREY, BACKGROUND_COLOR);

    //idk why this works but its somehow a good radius on the t-display, probably needs to be fixed for other displays
    timerRadius = tft.height()/4;

    timerCenterX = tft.width()/2;
    //bottom of the screen
    timerCenterY = tft.height() - timerRadius;

    barStart = 180-ARC_ANGLE/2;
    barEnd = 180+ARC_ANGLE/2;
    greenBarStart = 180-((ARC_ANGLE/2) * TARGET_SHOT_TIME_RANGE/TARGET_SHOT_TIME);
    greenBarEnd = 180+((ARC_ANGLE/2) * TARGET_SHOT_TIME_RANGE/TARGET_SHOT_TIME);
    yellowBarStart = 180-((ARC_ANGLE/2) * TARGET_SHOT_TIME_LESSER_RANGE/TARGET_SHOT_TIME);
    yellowBarEnd = 180+((ARC_ANGLE/2) * TARGET_SHOT_TIME_LESSER_RANGE/TARGET_SHOT_TIME);


    //temperature thing is on the top of the display
    tempRadius = timerRadius;
    tempCenterX = timerCenterX;
    tempCenterY = tempRadius + 10;

    targetTemperatureArcAngle = map(HX_TARGET_TEMPERATURE, HX_TEMP_LOWER_LIMIT, HX_TEMP_UPPER_LIMIT, 180-ARC_ANGLE/2, 180+ARC_ANGLE/2);
    targetTempStart = targetTemperatureArcAngle - map(HX_TARGET_TEMPERATURE_RANGE, 0, HX_TARGET_TEMPERATURE, 0, ARC_ANGLE);
    targetTempEnd = targetTemperatureArcAngle + map(HX_TARGET_TEMPERATURE_RANGE, 0, HX_TARGET_TEMPERATURE, 0, ARC_ANGLE);

    drawTimerArcOutline();
    drawTempArcOutline();
}


/// @brief Draws an arc based on the timer input, arc is constrained between 0 and 2 * TARGET_SHOT_TIME so that target is always on top
/// @param currentTime int, timer value in milliseconds
void MaraDisplayer::drawTimerArc(time_t currentTime)
{
    static int lastTime = 0;
    if (lastTime > currentTime) //reset the timer
        drawTimerArcOutline();
        
    static char buff[16];

    //constrain time to within the bar's limits
    currentTime = constrain(currentTime, 0, 2 * TARGET_SHOT_TIME * 1000);

    //linscale weighted bar length
    //add 1 to not have a 0 length bar causing shenanigans
    int timerBarEnd = map(currentTime, 0, 2 * TARGET_SHOT_TIME * 1000, barStart, barEnd) + 1;

    tft.drawSmoothArc(timerCenterX, timerCenterY, timerRadius - ARC_WIDTH/4, timerRadius - (ARC_WIDTH * 3)/4, barStart, timerBarEnd, TIMER_COLOR, TIMER_COLOR, true );
    sprintf(buff, "%02d.%d", currentTime / 1000, (currentTime % 1000) / 100);
    tft.drawCentreString(buff, timerCenterX, timerCenterY - tft.fontHeight(TIMER_FONT)/2, TIMER_FONT);
}

/// @brief Draws the outline for the timer dial/arc thing
void MaraDisplayer::drawTimerArcOutline()
{
    tft.drawSmoothArc(timerCenterX, timerCenterY, timerRadius, timerRadius - ARC_WIDTH, barStart, barEnd, TFT_LIGHTGREY, TFT_LIGHTGREY, true );
    tft.drawSmoothArc(timerCenterX, timerCenterY, timerRadius, timerRadius - ARC_WIDTH, yellowBarStart, yellowBarEnd, LESSER_TARGET_TIME_COLOR, LESSER_TARGET_TIME_COLOR, false );
    tft.drawSmoothArc(timerCenterX, timerCenterY, timerRadius, timerRadius - ARC_WIDTH, greenBarStart, greenBarEnd, TARGET_TIME_COLOR, TARGET_TIME_COLOR, false );
    
}

/// @brief Draws the outline for the temperature dial/arc thing
void MaraDisplayer::drawTempArcOutline()
{
    tft.drawSmoothArc(tempCenterX, tempCenterY, tempRadius, tempRadius - ARC_WIDTH, barStart, barEnd, TFT_LIGHTGREY, TFT_LIGHTGREY, true );
    //add in target arcs
    tft.drawSmoothArc(tempCenterX, tempCenterY, tempRadius, tempRadius - ARC_WIDTH, targetTempStart, targetTempEnd, TARGET_TIME_COLOR, TARGET_TIME_COLOR, false );

    tft.setTextSize(2);
    //draw a horizontal line between the above two strings
    tft.drawLine(tempCenterX - tft.textWidth("25.7 C")/2, tempCenterY, tempCenterX + tft.textWidth("25.7 C")/2, tempCenterY, TFT_LIGHTGREY);
    tft.setTextSize(3);
}


/// @brief Draws the temperature dial/arc thing, the arc dial is for the heat exchanger temperature but the steam boiler temperature is also printed
/// @param hxTemp int, heat exchanger temperature
/// @param steamTemp int, steam boiler temperature
void MaraDisplayer::drawTempArc(int hxTemp, int steamTemp)
{
    char buff[16];

    //record temperature from last call, if its less than the new temperature, redraw the outline
    static int lastTemp = 0;
    if (lastTemp > hxTemp)
        drawTempArcOutline();
    lastTemp = hxTemp;

    //constrain temp to be within the bar's limits
    hxTemp = constrain(hxTemp, HX_TEMP_LOWER_LIMIT, HX_TEMP_UPPER_LIMIT);

    //linscale weighted bar length
    //add 1 to not have a 0 length bar causing shenanigans
    int timerBarEnd = map(hxTemp, HX_TEMP_LOWER_LIMIT, HX_TEMP_UPPER_LIMIT, barStart, barEnd) + 1;
    tft.drawSmoothArc(tempCenterX, tempCenterY, tempRadius - ARC_WIDTH/4, tempRadius - (ARC_WIDTH * 3)/4, barStart, timerBarEnd, TIMER_COLOR, TIMER_COLOR, true );


    //print temperatures
    tft.setTextSize(2);
    sprintf(buff, "%3d C", hxTemp);
    tft.drawCentreString(buff, tempCenterX, tempCenterY - tft.fontHeight(TIMER_FONT) - 2, TIMER_FONT);
    sprintf(buff, "%3d C", steamTemp);
    tft.drawCentreString(buff, tempCenterX, tempCenterY + 4, TIMER_FONT);
    tft.setTextSize(3);
}


/// @brief Draw a small green dot on the lower left of the display whenever the heater is on
/// @param heaterOn boolean, true if heater is on
void MaraDisplayer::drawHeaterOnDot(bool heaterOn)
{
    tft.fillCircle(HEATER_DOT_SIZE + 2, tft.height() - HEATER_DOT_SIZE - 2, HEATER_DOT_SIZE, heaterOn ? TFT_GREEN : BACKGROUND_COLOR);
}


/// @brief Draws a single character in the lower right of the screen, shows whether mara is on steam or coffee priority mode
/// @param statusChar char to draw
void MaraDisplayer::drawStatusChar(char statusChar)
{
    tft.drawChar(tft.width() - tft.textWidth("X", TIMER_FONT)/2, tft.height() - tft.fontHeight(TIMER_FONT)/2, statusChar, TFT_WHITE, BACKGROUND_COLOR, TIMER_FONT);
}


/// @brief Calculates arc angle based on a chord, useful for figuring how wide an arc should be to fit a screen
/// @param radius radius of the arc
/// @param chord length of chord to fit
/// @return int, full angle of arc to fit chord in degrees
int MaraDisplayer::arcDegreeFromChord(int radius, int chord)
{
    return (acos(radius / sqrt(radius * radius + chord * chord)) * 180)/M_PI;
    
}