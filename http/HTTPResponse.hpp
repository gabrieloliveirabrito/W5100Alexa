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
    char body[ALEXA_MAX_BODY_LENGTH];
    HTTPStatusCode statusCode = NotImplemented;

public:
    HTTPResponse()
    {
        body[0] = '\0';
        setHeader("Content-Type", "text/plain");
    }

    ~HTTPResponse()
    {
        //Serial.println("Disposing HTTPResponse");
        dispose();
    }

    void dispose()
    {
        body[0] = '\0';
        headerCount = 0;
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
        // char str[ALEXA_MAX_BODY_LENGTH];
        // strncpy(str, body, ALEXA_MAX_BODY_LENGTH);
        // strncpy(this->body, str, ALEXA_MAX_BODY_LENGTH);
        strncpy(this->body, body, ALEXA_MAX_BODY_LENGTH);
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