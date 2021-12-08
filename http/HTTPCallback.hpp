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
    }

    virtual bool compare(HTTPRequest request)
    {
        DEBUG_PRINT(getMethod());
        DEBUG_PRINT(" <=> ");
        DEBUG_PRINTLN(request.getMethod());

        DEBUG_PRINT(getPath());
        DEBUG_PRINT(" <=> ");
        DEBUG_PRINTLN(request.getPath());

        return strcmp(getMethod(), request.getMethod()) == 0 && strcmp(getPath(), request.getPath()) == 0;
    }

    virtual bool isHeaderRequired(const char *name)
    {
        return false;
    }

    virtual bool isBodyRequired()
    {
        return false;
    }
};

#endif