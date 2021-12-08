#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#ifndef HTTP_MAX_RESPONSE_HEADERS
#define HTTP_MAX_RESPONSE_HEADERS 10
#endif

#ifndef HTTP_MAX_BODY_LENGTH
#define HTTP_MAX_BODY_LENGTH 512
#endif

#include "HTTPHeader.hpp"
#include "HTTPStatusCode.h"

class HTTPResponse
{
private:
    HTTPHeader headers[HTTP_MAX_RESPONSE_HEADERS];
    int headerCount = 0;
    char body[HTTP_MAX_BODY_LENGTH];
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

        if (headerCount <= HTTP_MAX_RESPONSE_HEADERS)
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
        if (position >= HTTP_MAX_RESPONSE_HEADERS)
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
        // char str[HTTP_MAX_BODY_LENGTH];
        // strncpy(str, body, HTTP_MAX_BODY_LENGTH);
        // strncpy(this->body, str, HTTP_MAX_BODY_LENGTH);
        strncpy(this->body, body, HTTP_MAX_BODY_LENGTH);
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