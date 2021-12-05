#ifndef ETHERNET_ALEXA_DEVICE
#define ETHERNET_ALEXA_DEVICE

#ifndef ALEXA_DEVICE_NAME_SIZE
#define ALEXA_DEVICE_NAME_SIZE 30
#endif

class EthernetAlexaDevice
{
private:
    char name[ALEXA_DEVICE_NAME_SIZE];
    uint8_t id;

public:
    EthernetAlexaDevice(const char *name)
    {
        strncpy(this->name, name, ALEXA_DEVICE_NAME_SIZE);
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
        strncpy(this->name, name, ALEXA_DEVICE_NAME_SIZE);
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
    virtual const char *getJson() = 0;
    virtual void doCallback() = 0;
};

#endif