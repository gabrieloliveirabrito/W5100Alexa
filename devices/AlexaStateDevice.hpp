#ifndef ALEXA_STATE_DEVICE
#define ALEXA_STATE_DEVICE

#include "EthernetAlexaDevice.hpp"

class AlexaStateDevice : public EthernetAlexaDevice
{
private:
    bool state;

public:
    AlexaStateDevice(const char *name) : EthernetAlexaDevice(name)
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
};

#endif