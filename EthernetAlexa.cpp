#include "EthernetAlexa.h"
#include "templates.h"
#include "httpCallbacks/include.h"

void httpR(HTTPRequest *request, HTTPResponse *response) {}

bool EthernetAlexa::begin()
{
#ifdef ALEXA_DEBUG_PIN
    pinmode(ALEXA_DEBUG_PIN, OUTPUT);
    digitalWrite(ALEXA_DEBUG_PIN, LOW);
#endif
    ip = Ethernet.localIP();
    Ethernet.MACAddress(mac);
    sprintf(escapedMac, "FD-%x%x%x%x%x%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    for (int i = 0; i < sizeof(uint32_t); i++)
        mac24 |= mac[i] << i * 8;

    http = new HTTPServer();
    http->addHttpCallback(new HTTPIndexCallback(this));
    // http->addHttpCallback(new DescriptionXMLCallback(this));
    // http->addHttpCallback(new APILightsCallback(this));
    // http->addHttpCallback(new APIControlsCallback(this));
    http->begin();

    udp = new EthernetUDP();
    connected = udp->beginMulticast(IPAddress(239, 255, 255, 250), 1900);

    return connected;
}

void EthernetAlexa::loop()
{
    http->loop();

    if (!connected)
    {
        Serial.println("Not connected");
        return;
    }

    int packetSize = udp->parsePacket();
    if (packetSize > 0)
    {
        unsigned char packetBuffer[packetSize + 1];
        udp->read(packetBuffer, packetSize);
        packetBuffer[packetSize] = 0;
        udp->flush();

        if (!discoverable)
        {
            return;
        }

        char *request = (char *)packetBuffer;
        //Serial.println(request);

        if (strstr(request, "M-SEARCH") != nullptr || strstr(request, "NOTIFY") != nullptr)
        {

            if (strstr(request, "ssdp:discover") != nullptr || strstr(request, "upnp:rootdevice") != nullptr ||
                strstr(request, "device:basic:1") != nullptr || strstr(request, "ssdp:disc") != nullptr ||
                strstr(request, "upnp:rootd") != nullptr || strstr(request, "sspd:all") != nullptr ||
                strstr(request, "asic:1") != nullptr || strstr(request, "ssdp:alive") != nullptr)
            {
#ifdef ALEXA_DEBUG_PIN
                digitalWrite(ALEXA_DEBUG_PIN, HIGH);
#endif

                char s[16];
                sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

                char buf[strlen(HTTP_HEADERS) + 128];
                sprintf_P(buf, HTTP_HEADERS, s, http->getPort(), escapedMac, escapedMac);

                udp->beginPacket(udp->remoteIP(), udp->remotePort());
                udp->write(buf);
                udp->endPacket();

                delay(200);
#ifdef ALEXA_DEBUG_PIN
                digitalWrite(ALEXA_DEBUG_PIN, LOW);
#endif
            }
            else
            {
                Serial.println(request);
            }
        }
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

char *EthernetAlexa::getEscapedMac()
{
    return escapedMac;
}

uint8_t EthernetAlexa::getDeviceCount()
{
    return deviceCount;
}

uint8_t EthernetAlexa::getDeviceId(char *device_name)
{
    for (int i = 0; i < deviceCount; i++)
    {
        EthernetAlexaDevice *device = devices[i];
        if (strcmp(device->getName(), device_name) == 0)
            return i;
    }
    return -1;
}

EthernetAlexaDevice *EthernetAlexa::getDevice(uint8_t id)
{
    if (id < deviceCount)
        return devices[id];
    else
        return nullptr;
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

void EthernetAlexa::renameDevice(uint8_t id, char *device_name)
{
    if (id < deviceCount)
        devices[id]->setName(device_name);
}