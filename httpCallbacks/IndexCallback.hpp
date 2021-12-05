#ifndef ALEXA_INDEX_CALLBACK
#define ALEXA_INDEX_CALLBACK

#include "../http/HTTPCallback.hpp"
#include "../EthernetAlexa.h"

PROGMEM const char BODY_HTML[] = "<html><head><title>Ethernet Alexa</title></head>"
                                 "<body>"
                                 "<h3>Devices: %d</h3>"
                                 "<h3>UDP Connected: %s</h3>"
                                 "</body></html>";

class HTTPIndexCallback : public HTTPCallback
{
private:
    EthernetAlexa *alexa;

public:
    HTTPIndexCallback(EthernetAlexa *alexa) : HTTPCallback()
    {
        this->alexa = alexa;
    }

    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/";
    }

    void execute(HTTPRequest request, HTTPResponse &response)
    {
        Serial.println("Index called!");

        response.setStatusCode(OK);
        response.setHeader("Content-Type", "text/html");
        //response.setBody(buf);
    }
};

#endif