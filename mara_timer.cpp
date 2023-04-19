#include <Arduino.h>

#include "pins.h"
#include "mara_timer.h"

bool isTimePinEnabled()
{
    return !digitalRead(PUMP_PIN);
}

//a debouncing algorithm cause reed switch can be finnicky.
//function is unused because attaching a capacitor fixed the issues
bool debouncedPumpState()
{
    static bool prevState = false;
    static int stateConsecutiveCounter = 0;
    static bool debouncedState = false;
    
    bool currentState = isTimePinEnabled();

    if(currentState != prevState)
    {
        stateConsecutiveCounter = 0;
        prevState = currentState;
    }
    else
    {
        stateConsecutiveCounter++;
        if(stateConsecutiveCounter = DEBOUNCE_THRESH)
        {
            debouncedState = currentState;
        }
    }
    return debouncedState;
}

time_t stopwatch()
{
    static bool timerEnabled = false;
    static time_t startTime = 0;
    static time_t endTime = 0;
    if (isTimePinEnabled())
    {
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
