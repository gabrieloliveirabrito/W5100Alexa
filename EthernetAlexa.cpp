#include "EthernetAlexa.h"
#include "templates.h"
#include "httpCallbacks/include.h"

void httpR(HTTPRequest request, HTTPResponse *response) {}

bool EthernetAlexa::begin()
{
    ip = Ethernet.localIP();
    Ethernet.MACAddress(mac);
    sprintf(escapedMac, "%x%x%x%x%x%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    for (int i = 0; i < sizeof(uint32_t); i++)
        mac24 |= mac[i] << i * 8;

    http = new HTTPServer();
    http->addHttpCallback(new HTTPIndexCallback(this));
    http->addHttpCallback(new DescriptionXMLCallback(this));
    http->addHttpCallback(new APICallback(this));
    http->begin();

    udp = new EthernetUDP();
    connected = udp->beginMulticast(IPAddress(239, 255, 255, 250), 1900);

    return connected;
}

void EthernetAlexa::loop()
{
    http->loop();

    if (!connected)
        return;

    int packetSize = udp->parsePacket();
    if (packetSize < 1)
        return;

    unsigned char packetBuffer[packetSize + 1];
    udp->read(packetBuffer, packetSize);
    packetBuffer[packetSize] = 0;
    udp->flush();

    if (!discoverable)
        return;

    const char *request = (const char *)packetBuffer;
    if (strstr(request, "M-SEARCH") == nullptr)
        return;

    if (strstr(request, "ssdp:discover") != nullptr || strstr(request, "upnp:rootdevice") != nullptr || strstr(request, "device:basic:1") != nullptr)
    {
        Serial.println("Alexa Received!");

        char s[16];
        sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

        char buf[strlen(HTTP_HEADERS) + 128];
        sprintf_P(buf, HTTP_HEADERS, s, escapedMac, escapedMac);

        udp->beginPacket(udp->remoteIP(), udp->remotePort());
        udp->write(buf);
        udp->endPacket();
    }
    else
    {
        Serial.println("Invalid Packet");
        Serial.println(request);
    }
}

bool EthernetAlexa::isConnected()
{
    return connected;
}

bool EthernetAlexa::isDiscoverable()
{
    return discoverable;
}

void EthernetAlexa::setDiscoverable(bool discoverable)
{
    this->discoverable = discoverable;
}

IPAddress EthernetAlexa::getLocalIP()
{
    return ip;
}

const char *EthernetAlexa::getEscapedMac()
{
    return escapedMac;
}

uint8_t EthernetAlexa::getDeviceCount()
{
    return deviceCount;
}

uint8_t EthernetAlexa::getDeviceId(const char *device_name)
{
    for (int i = 0; i < deviceCount; i++)
    {
        EthernetAlexaDevice *device = devices[i];
        if (strcmp(device->getName(), device_name) == 0)
            return i;
    }
    return -1;
}

uint8_t EthernetAlexa::addDevice(EthernetAlexaDevice *device)
{
    if (deviceCount >= ALEXA_MAX_DEVICES)
        return -1;
    if (device == nullptr)
        return -2;

    device->setId(deviceCount);
    devices[deviceCount++] = device;

    return deviceCount;
}

void EthernetAlexa::renameDevice(uint8_t id, const char *device_name)
{
    if (id < deviceCount)
        devices[id]->setName(device_name);
}