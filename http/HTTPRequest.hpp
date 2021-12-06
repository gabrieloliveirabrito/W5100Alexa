#ifndef ALEXA_HTTP_REQUEST
#define ALEXA_HTTP_REQUEST
#endif

#ifndef ALEXA_MAX_REQUEST_HEADERS
#define ALEXA_MAX_REQUEST_HEADERS 5
#endif

#ifndef ALEXA_MAX_METHOD_SIZE
#define ALEXA_MAX_METHOD_SIZE 12
#endif

#ifndef ALEXA_MAX_PATH_SIZE
#define ALEXA_MAX_PATH_SIZE 64
#endif

#ifndef ALEXA_MAX_BODY_SIZE
#define ALEXA_MAX_BODY_SIZE 256

#include "HTTPHeader.hpp"

class HTTPRequest
{
private:
    HTTPHeader headers[ALEXA_MAX_REQUEST_HEADERS];
    int headerCount = 0;

    char method[ALEXA_MAX_METHOD_SIZE];
    char path[ALEXA_MAX_PATH_SIZE];
    char body[ALEXA_MAX_BODY_SIZE];

public:
    HTTPRequest()
    {
        method[0] = '\0';
        path[0] = '\0';
        body[0] = '\0';
    }

    ~HTTPRequest()
    {
        //Serial.println("Disposing HTTPRequest");
        dispose();
    }

    void dispose()
    {
        method[0] = '\0';
        path[0] = '\0';
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

        if (headerCount <= ALEXA_MAX_REQUEST_HEADERS)
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
        if (position >= ALEXA_MAX_REQUEST_HEADERS)
            return nullptr;
        else
            return &headers[position];
    }

    const char *getMethod()
    {
        return method;
    }

    void setMethod(char method[], int len)
    {
        strncpy(this->method, method, len);
    }

    const char *getPath()
    {
        return path;
    }

    void setPath(char path[], int len)
    {
        strncpy(this->path, path, len);
    }

    const char *getBody()
    {
        return body;
    }

    void setBody(char body[], int len)
    {
        strncpy(this->body, body, len);
    }
};

#endif