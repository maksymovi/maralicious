#pragma once

#define BUFFER_SIZE 64

// C1.19,116,124,095,0560,0
typedef struct MaraXOutput
{
    char steamOrBrew;
    int version;
    int steamTemp;
    int targetTemp;
    int hxTemp;
    int heatingCountdown;
    bool heaterOn;

    char completeString[BUFFER_SIZE];
    time_t time;
} MaraXOutput;

bool readMaraSerial(MaraXOutput &out);