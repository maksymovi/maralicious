#pragma once

class MaraTimer
{
    public:
        MaraTimer(int pumpSensorPin);
        bool isPumpEnabled();
        long stopwatch();
    private:
        int pumpSensorPin;
        bool timerEnabled = false;
        long startTime = 0;
        long endTime = 0;
};