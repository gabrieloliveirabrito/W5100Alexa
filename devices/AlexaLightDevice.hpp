#ifndef ALEXA_LIGHT_DEVICE
#define ALEXA_LIGHT_DEVICE

#include "EthernetAlexaDevice.hpp"

class AlexaLightDevice : public EthernetAlexaDevice
{
private:
    bool state;
    int brightness = 0;

public:
    AlexaLightDevice(const char *name) : EthernetAlexaDevice(name) {}

    bool getState()
    {
        return state;
    }

    void setState(bool state)
    {
        this->state = state;
    }

    uint8_t getBrightness()
    {
        return brightness;
    }

    void setBrightness(uint8_t brightness)
    {
        this->brightness = brightness;
    }

    uint8_t toPercentage()
    {
        uint16_t percentage = brightness * 100;
        return percentage / 255;
    }

    const char *getType()
    {
        return "Dimmable light";
    }

    const char *getModelId()
    {
        return "LWT010";
    }

    void doCallback() {}
};

#endif