#pragma once
#define BUFFER_SIZE 64

#include <string>

/// @brief Singleton which reads from the mara serial port and parses the output
class MaraSerialReader
{
public:
    // C1.19,116,124,095,0560,0
    /// @brief Struct containing the output of the Mara serial port
    typedef struct MaraXOutput
    {
        char steamOrBrew;
        int version;
        int steamTemp;
        int targetTemp;
        int hxTemp;
        int heatingCountdown;
        bool heaterOn;

        std::string completeString = "";
        time_t time;

    } MaraXOutput;

    /// @brief This is a singleton, should not be clonable, use getInstance() instead
    MaraSerialReader(MaraSerialReader &other) = delete;

    /// @brief This is a singleton, should not be assignable, use getInstance() instead
    void operator=(const MaraSerialReader &) = delete;

    static MaraSerialReader* getInstance();

    bool readMaraSerial(MaraXOutput &MaraXOutput);

protected:
    MaraSerialReader();


private:
    static MaraSerialReader* instance;
    std::string stringToParse = "";

    bool parseSerial(MaraXOutput &out);

};