#ifndef ALEXA_API_CALLBACK
#define ALEXA_API_CALLBACK

#include "../http/HTTPCallback.hpp"
#include "../EthernetAlexa.h"

class APICallback : public HTTPCallback
{
private:
    EthernetAlexa *alexa;

public:
    APICallback(EthernetAlexa *alexa) : HTTPCallback()
    {
        this->alexa = alexa;
    }

    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/api/lights/";
    }

    void execute(HTTPRequest request, HTTPResponse *response)
    {
        int id = atoi(request.getPath() + 12);

        char buf[1024];
        sprintf(buf, "<html><h1>%d</h1></html>", id);
        Serial.println(buf);

        response->setStatusCode(OK);
        response->setBody(buf);
    }

    bool compare(const char *method, const char *path)
    {
        Serial.println(path);
        return strcmp(getMethod(), method) == 0 && strncmp(path, getPath(), 12) == 0;
    }
};

#endif