#ifndef ETHERNET_ALEXA_DEVICE
#define ETHERNET_ALEXA_DEVICE

class EthernetAlexaDevice
{
private:
    const char *name;
    uint8_t id;

public:
    EthernetAlexaDevice(const char *name)
    {
        this->name = name;
    }

    ~EthernetAlexaDevice()
    {
    }

    const char *getName()
    {
        return name;
    }

    void setName(const char *name)
    {
        this->name = name;
    }

    uint8_t getId()
    {
        return id;
    }

    void setId(uint8_t id)
    {
        this->id = id;
    }

    virtual const char *getType() = 0;
    virtual const char *getModelId() = 0;
    virtual void doCallback() = 0;
};

#endif