#ifndef HTTP_CALLBACK
#define HTTP_CALLBACK

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPCallback
{
public:
    virtual const char *getMethod() = 0;
    virtual const char *getPath() = 0;

    virtual void execute(HTTPRequest request, HTTPResponse *response) = 0;
    virtual bool compare(const char *method, const char *path)
    {
        return strcmp(getMethod(), method) == 0 && strcmp(getPath(), path) == 0;
    }
};

#endif