#ifndef ALEXA_HTTP_RESPONSE
#define ALEXA_HTTP_RESPONSE

#ifndef ALEXA_MAX_RESPONSE_HEADERS
#define ALEXA_MAX_RESPONSE_HEADERS 10
#endif

#ifndef ALEXA_MAX_BODY_LENGTH
#define ALEXA_MAX_BODY_LENGTH 512
#endif

#include "HTTPHeader.hpp"
#include "HTTPStatusCode.h"

class HTTPResponse
{
private:
    HTTPHeader headers[ALEXA_MAX_RESPONSE_HEADERS];
    int headerCount = 0;
    const char *body;
    HTTPStatusCode statusCode;

public:
    HTTPResponse()
    {
        body = nullptr;
        setHeader("Content-Type", "text/plain");
    }

    ~HTTPResponse()
    {
        //dispose();
    }

    void dispose()
    {
        if (headers != nullptr && headerCount > 0)
            for (int i = 0; i < headerCount; i++)
                headers[i].dispose();
        headerCount = 0;

        if (body != nullptr)
            delete body;
    }

    int getHeaderCount()
    {
        return headerCount;
    }

    void setHeader(const char *name, const char *value)
    {
        if (name == nullptr || value == nullptr)
            return;

        for (int i = 0; i < headerCount; i++)
        {
            HTTPHeader *header = &headers[i];

            if (strcmp(header->getName(), name) == 0)
            {
                header->setValue(value);
                return;
            }
        }

        if (headerCount <= ALEXA_MAX_RESPONSE_HEADERS)
            headers[headerCount++] = HTTPHeader(name, value);
    }

    const char *getHeader(const char *name)
    {
        if (name != nullptr)
        {
            for (int i = 0; i < headerCount; i++)
            {
                HTTPHeader *header = &headers[i];

                if (strcmp(header->getName(), name) == 0)
                {
                    return header->getValue();
                }
            }
        }
        return nullptr;
    }

    HTTPHeader *getHeader(uint8_t position)
    {
        if (position >= ALEXA_MAX_RESPONSE_HEADERS)
            return nullptr;
        else
            return &headers[position];
    }

    const char *getBody()
    {
        return body;
    }

    void setBody(const char *body)
    {
        this->body = body;
    }

    HTTPStatusCode getStatusCode()
    {
        return statusCode;
    }

    void setStatusCode(HTTPStatusCode statusCode)
    {
        this->statusCode = statusCode;
    }
};

#endif