#include <Arduino.h>

#include "mara_serial.h"


bool readMaraSerial(MaraXOutput &out)
{
    int val;
    int versionTemp1;
    int versionTemp2;
    int heaterTemp;
    if (Serial2.available())
    {
        val = Serial2.read(out.completeString, BUFFER_SIZE);

        sscanf(out.completeString, "%c%d.%d,%d,%d,%d,%d,%d",
               &out.steamOrBrew, &versionTemp1, &versionTemp2,
               &out.steamTemp, &out.targetTemp, &out.hxTemp, &out.heatingCountdown, &heaterTemp);
        out.version = versionTemp1 * 100 + versionTemp2;
        out.heaterOn = heaterTemp;
        return val == 8; // return true if all 8 vales were read
    }
    else
    {
        return false;
    }
}
