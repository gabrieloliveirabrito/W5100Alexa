#ifndef ALEXA_API_LIGHTS_CALLBACK
#define ALEXA_API_LIGHTS_CALLBACK

#include "../http/HTTPCallback.hpp"
#include "../EthernetAlexa.h"

class APILightsCallback : public HTTPCallback
{
private:
    EthernetAlexa *alexa;

public:
    APILightsCallback(EthernetAlexa *alexa) : HTTPCallback()
    {
        this->alexa = alexa;
    }

    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/api/lights";
    }

    void execute(HTTPRequest request, HTTPResponse &response)
    {
        int id = atoi(request.getPath() + 12);
        String url = String(request.getPath());

        int pos = url.indexOf("");
        if (pos == -1)
        {
            response.setStatusCode(NotFound);
        }
        else
        {
            Serial.print("ID ");
            Serial.println(id, DEC);

            if (0 == id)
            {
                for (uint8_t i = 0, n = alexa->getDeviceCount(); i < n; i++)
                {
                    EthernetAlexaDevice *device = alexa->getDevice(i);
                    response.setBody(device->getJson());
                }
            }
            else
            {
                EthernetAlexaDevice *device = alexa->getDevice(id - 1);
                response.setBody(device->getJson());
            }

            response.setHeader("Content-Type", "application/json");
            response.setStatusCode(OK);
        }
    }

    bool compare(HTTPRequest request)
    {
        const char *p = getPath();
        return strcmp(getMethod(), request.getMethod()) == 0 && strncmp(request.getPath(), p, strlen(p)) == 0;
    }
};

#endif