#ifndef ALEXA_API_CONTROLS_CALLBACK
#define ALEXA_API_CONTROLS_CALLBACK

#include "../http/HTTPCallback.hpp"
#include "../EthernetAlexa.h"

class APIControlsCallback : public HTTPCallback
{
private:
    EthernetAlexa *alexa;

public:
    APIControlsCallback(EthernetAlexa *alexa) : HTTPCallback()
    {
        this->alexa = alexa;
    }

    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/api/";
    }

    void execute(HTTPRequest request, HTTPResponse &response)
    {
        String body = String(request.getBody());

        response.setHeader("Content-Type", "application/json");
        if (body.indexOf("deviceType") > 0)
        {
            response.setStatusCode(OK);
            response.setBody("[{\"success\":{\"username\":\"t4bK6SilLBy4DutQo0P8yORztUFrAf5-NDTi7wFq\"}}]");
        }
        else
        {
            response.setStatusCode(NotFound);
        }
    }

    bool compare(HTTPRequest request)
    {
        Serial.println(strncmp(request.getPath(), getPath(), 4) == 0 ? "true" : "false");
        return strncmp(request.getPath(), getPath(), 4) == 0;
    }
};

#endif