#include <Arduino.h>

#include "mara_serial.h"


// C1.19,116,124,095,0560,0
bool readMaraSerial(MaraXOutput &out)
{
    int val;
    int versionTemp1;
    int versionTemp2;
    int heaterTemp;
    if (Serial2.available())
    {
        val = Serial2.read(out.completeString, BUFFER_SIZE);

        return parseMaraSerial(out, out.completeString);
    }
    else
    {
        return false;
    }
}

bool parseMaraSerial(MaraXOutput &out, char *input)
{
    int val;
    int versionTemp1;
    int versionTemp2;
    int heaterTemp;
    val = sscanf(input, "%c%d.%d,%d,%d,%d,%d,%d",
                 &out.steamOrBrew, &versionTemp1, &versionTemp2,
                 &out.steamTemp, &out.targetTemp, &out.hxTemp, &out.heatingCountdown, &heaterTemp);
    out.version = versionTemp1 * 100 + versionTemp2;
    out.heaterOn = heaterTemp;
    return val == 8; // return true if all 8 values were read
}