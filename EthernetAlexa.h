//This code/library is based on Espalexa
//If your use ESP Wifi Library, access https://github.com/Aircoookie/Espalexa/
#ifndef ETHERNET_ALEXA
#define ETHERNET_ALEXA

#ifndef ALEXA_MAX_DEVICES
#define ALEXA_MAX_DEVICES 10
#endif

#include <EthernetUdp.h>
#include "devices\include.h"
#include "http\include.h"

class EthernetAlexa
{
private:
    HTTPServer *http;
    EthernetUDP *udp;
    uint8_t deviceCount = 0;
    bool discoverable = true;
    bool connected = false;

    EthernetAlexaDevice *devices[ALEXA_MAX_DEVICES];
    IPAddress ip;
    byte mac[6];
    uint32_t mac24;
    char escapedMac[12];

public:
    bool begin();
    void loop();
    bool isConnected();

    bool isDiscoverable();
    void setDiscoverable(bool discoverable);

    IPAddress getLocalIP();
    const char *getEscapedMac();

    uint8_t getDeviceCount();
    uint8_t getDeviceId(const char *device_name);
    uint8_t addDevice(EthernetAlexaDevice *device);
    void renameDevice(uint8_t id, const char *device_name);
};

#endif