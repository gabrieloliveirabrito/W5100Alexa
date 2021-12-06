#ifndef HTTP_CALLBACK
#define HTTP_CALLBACK

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPCallback
{
public:
    virtual const char *getMethod() = 0;
    virtual const char *getPath() = 0;
    virtual void execute(HTTPRequest *request, HTTPResponse *response) = 0;

    virtual void dispose()
    {
        //Serial.println("Disposing callback!");
    }

    virtual bool compare(HTTPRequest request)
    {
        Serial.print(getMethod());
        Serial.print(" <=> ");
        Serial.print(request.getMethod());
        Serial.print(" = ");
        Serial.println(strcmp(getMethod(), request.getMethod()), DEC);

        Serial.print(getPath());
        Serial.print(" <=> ");
        Serial.print(request.getPath());
        Serial.print(" = ");
        Serial.println(strcmp(getPath(), request.getPath()), DEC);

        return strcmp(getMethod(), request.getMethod()) == 0 && strcmp(getPath(), request.getPath()) == 0;
    }

    virtual bool isHeaderRequired(const char *name)
    {
        return false;
    }
};

#endif