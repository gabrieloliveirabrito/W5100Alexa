#ifndef HTTP_REQUEST
#define HTTP_REQUEST
#endif

#ifndef HTTP_MAX_REQUEST_HEADERS
#define HTTP_MAX_REQUEST_HEADERS 5
#endif

#ifndef HTTP_MAX_METHOD_SIZE
#define HTTP_MAX_METHOD_SIZE 12
#endif

#ifndef HTTP_MAX_PATH_SIZE
#define HTTP_MAX_PATH_SIZE 64
#endif

#ifndef HTTP_MAX_BODY_SIZE
#define HTTP_MAX_BODY_SIZE 512

#include "HTTPHeader.hpp"

class HTTPRequest
{
private:
    HTTPHeader headers[HTTP_MAX_REQUEST_HEADERS];
    int headerCount = 0;
    bool headersRequired = true;

    char method[HTTP_MAX_METHOD_SIZE];
    char path[HTTP_MAX_PATH_SIZE];
    char body[HTTP_MAX_BODY_SIZE];

public:
    HTTPRequest(bool headersRequired = true)
    {
        this->headersRequired = headersRequired;

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

    bool isHeadersRequired()
    {
        return headersRequired;
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

        if (headerCount <= HTTP_MAX_REQUEST_HEADERS)
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
        if (position >= HTTP_MAX_REQUEST_HEADERS)
            return nullptr;
        else
            return &headers[position];
    }

    const char *getMethod()
    {
        return method;
    }

    void setMethod(char method[])
    {
        setMethod(method, HTTP_MAX_METHOD_SIZE);
    }

    void setMethod(char method[], int len)
    {
        strncpy(this->method, method, len);
    }

    const char *getPath()
    {
        return path;
    }

    void setPath(char path[])
    {
        setPath(path, HTTP_MAX_PATH_SIZE);
    }

    void setPath(char path[], int len)
    {
        strncpy(this->path, path, len);
    }

    const char *getBody()
    {
        return body;
    }

    void setBody(char body[])
    {
        setBody(body, HTTP_MAX_BODY_SIZE);
    }

    void setBody(char body[], int len)
    {
        strncpy(this->body, body, len);
    }
};

#endif