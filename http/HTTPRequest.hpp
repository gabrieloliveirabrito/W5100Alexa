#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include "HTTPHeader.hpp"

class HTTPRequest
{
private:
    HTTPHeader headers[HTTP_MAX_REQUEST_HEADERS];
    int headerCount = 0;
    bool headersRequired = true, bodyRequired = false;

    char method[HTTP_MAX_METHOD_LENGTH];
    char path[HTTP_MAX_PATH_LENGHT];
    char body[HTTP_MAX_BODY_LENGTH];

public:
    HTTPRequest()
    {
        method[0] = '\0';
        path[0] = '\0';
        body[0] = '\0';
    }

    ~HTTPRequest()
    {
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

    void setHeadersRequired(bool headersRequired)
    {
        this->headersRequired = headersRequired;
    }

    bool isBodyRequired()
    {
        return bodyRequired;
    }

    void setBodyRequired(bool bodyRequired)
    {
        this->bodyRequired = bodyRequired;
    }

    int getHeaderCount()
    {
        return headerCount;
    }

    void setHeader(const char *name, const char *value)
    {
        if (name == nullptr || value == nullptr || !headersRequired)
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
        setMethod(method, HTTP_MAX_METHOD_LENGTH);
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
        setPath(path, HTTP_MAX_PATH_LENGHT);
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
        setBody(body, HTTP_MAX_BODY_LENGTH);
    }

    void setBody(char body[], int len)
    {
        if (!bodyRequired)
            return;

        strncpy(this->body, body, len);
    }
};

#endif