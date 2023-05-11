#include "MaraTimer.h"

#include <Arduino.h>

#include "config/debug.h"

MaraTimer::MaraTimer(int pumpSensorPin)
{
    this->pumpSensorPin = pumpSensorPin;
    pinMode(pumpSensorPin, INPUT_PULLUP);
}

/// @brief checks if the set timer pin is enabled
/// @return true if the timer pin is enabled, false otherwise
bool MaraTimer::isPumpEnabled()
{
    return !digitalRead(pumpSensorPin);
}


//may be wise to add a debouncing function but a capacitor across the reed switch seems to fix bouncing

/// @brief checks if pump is enabled and runs a timer for its duration
/// @return the time in milliseconds since pump was enabled
long MaraTimer::stopwatch()
{
#ifdef TEST_DEBUG
    return millis();
#endif
    if (isPumpEnabled())
    {
#ifdef LIVE_DEBUG
        Serial.println("Time Pin Enabled");
#endif
        if(!timerEnabled) //timer becomes enabled
        {
            startTime = millis();
            timerEnabled = true;
            return 0;
        }
        endTime = millis();
    }
    else
    {
        if(timerEnabled) //disable timer
            timerEnabled = 0;
    }
    return endTime -  startTime;
}

