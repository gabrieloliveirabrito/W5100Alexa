#ifndef HTTP_CALLBACK
#define HTTP_CALLBACK

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPCallback
{
public:
    virtual const char *getMethod() = 0;
    virtual const char *getPath() = 0;

    virtual void execute(HTTPRequest request, HTTPResponse &response) = 0;
    virtual bool compare(HTTPRequest request)
    {
        const char *method = request.getMethod();
        const char *path = request.getPath();

        Serial.print("Comparing ");
        if (method == nullptr)
            Serial.print("NULL");
        else
            Serial.print(method);
        Serial.print(" to ");
        if (getMethod() == nullptr)
            Serial.print("NULL");
        else
            Serial.print(getMethod());
        Serial.println();

        return false;
        //return strcmp(getMethod(), request.getMethod()) == 0 && strcmp(getPath(), request.getPath()) == 0;
    }
};

#endif