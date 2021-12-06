#ifndef ALEXA_STATE_DEVICE
#define ALEXA_STATE_DEVICE

#include "EthernetAlexaDevice.hpp"
#include "../templates.h"

class AlexaStateDevice : public EthernetAlexaDevice
{
private:
    bool state;

public:
    AlexaStateDevice(char *name) : EthernetAlexaDevice(name)
    {
    }

    bool getState()
    {
        return state;
    }

    void setState(bool state)
    {
        this->state = state;
    }

    void doCallback()
    {
    }

    const char *getType()
    {
        return "Dimmable light";
    }

    const char *getModelId()
    {
        return "LWT010";
    }

    const char *getJson()
    {
        char buffer[strlen_P(DEVICE_JSON_TEMPLATE) + 64];
        snprintf_P(buffer, sizeof(buffer), DEVICE_SHORT_JSON_TEMPLATE, getName(), getModelId(), state ? "true" : "false");

        return buffer;
    }
};

#endif