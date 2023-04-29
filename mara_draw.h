#pragma once

#include <TFT_eSPI.h>
#include <SPI.h>
#include <User_Setup_Select.h>

#define BACKGROUND_COLOR TFT_BLACK

extern TFT_eSPI tft;

void drawSetup();
void drawTimerArcOutline();
void drawTimerArc(time_t currentTime);
void drawTempArcOutline();
void drawTempArc(int hxTemp, int steamTemp);
int arcDegreeFromChord(int radius, int chord);