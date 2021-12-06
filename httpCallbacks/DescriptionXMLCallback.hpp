#ifndef ALEXA_DESCRIPTIONXML_CALLBACK
#define ALEXA_DESCRIPTIONXML_CALLBACK

#include "../http/HTTPCallback.hpp"
#include "../EthernetAlexa.h"
#include "../templates.h"

class DescriptionXMLCallback : public HTTPCallback
{
private:
    EthernetAlexa *alexa;

public:
    DescriptionXMLCallback(EthernetAlexa *alexa) : HTTPCallback()
    {
        this->alexa = alexa;
    }

    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/description.xml";
    }

    void execute(HTTPRequest *request, HTTPResponse *response)
    {
        IPAddress localIP = alexa->getLocalIP();
        char *escapedMac = alexa->getEscapedMac();

        char s[16];
        sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

        char buf[1024];
        sprintf_P(buf, DESCRIPTION_XML, s, s, escapedMac, escapedMac);

        response->setStatusCode(OK);
        response->setHeader("Content-Type", "text/xml");
        response->setBody(buf);
    }
};

#endif