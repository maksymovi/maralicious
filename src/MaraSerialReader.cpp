#include "MaraSerialReader.h"

#include <Arduino.h>
#include <regex>

#include "config/pins.h"
#include "config/debug.h"

MaraSerialReader* MaraSerialReader::instance = nullptr;

/// @brief Constructor for MaraSerialReader, private because this is a singleton
MaraSerialReader::MaraSerialReader()
{
    Serial2.begin(9600, SERIAL_8N1, MARAX_RX_PIN, MARAX_TX_PIN);
}

/// @brief Singleton getter for MaraSerialReader, call this to get an instance
/// @return Pointer to the MaraSerialReader singleton
MaraSerialReader* MaraSerialReader::getInstance()
{
    if (instance == nullptr)
    {
        instance = new MaraSerialReader();
    }
    return instance;
}

/// @brief Reads the mara serial port and parses the output
/// @param maraXOutput MaraXOutput object to place the output into if found
/// @return True if a full output was found, false otherwise
bool MaraSerialReader::readMaraSerial(MaraXOutput &maraXOutput)
{
    char buffer[BUFFER_SIZE] = "";
    
#ifndef DEBUG
    Serial2.read(buffer, BUFFER_SIZE);
#else
    strcpy(buffer, "C1.19,116,124,095,0560,0");
#endif
    
    //we append and parse in case it takes multiple calls to the serial port to get a full output
    stringToParse += buffer;

    return parseSerial(maraXOutput);

}

// C1.19,116,124,095,0560,0
static const std::regex maraXRegexPattern("(C|V)1\\.(\\d{2}),(\\d{3}),(\\d{3}),(\\d{3}),(\\d{4}),(0|1)");

/// @brief Parse stringToParse and place the output into out
/// @param out filled with the output if found
/// @return if the correct string was found and out was filled or not
bool MaraSerialReader::parseSerial(MaraXOutput &out)
{
    
    std::smatch match;
    if (std::regex_search(stringToParse, match, maraXRegexPattern)) {
        out.time = millis();

        out.steamOrBrew = match[1].str()[0];
        out.version = std::stoi(match[2].str());
        out.steamTemp = std::stoi(match[3].str());
        out.targetTemp = std::stoi(match[4].str());
        out.hxTemp = std::stoi(match[5].str());
        out.heatingCountdown = std::stoi(match[6].str());
        out.heaterOn = std::stoi(match[7].str());
        

        
        out.completeString = match.str();
        //whenever we find a match we wipe stringToParse, but if we don't we keep it because the serial data may come in chunks
        stringToParse.clear();

        return true;
    } else {
        //return false if no match is found, still fill the completeString though
        out.completeString = stringToParse;
        return false;
    }
}