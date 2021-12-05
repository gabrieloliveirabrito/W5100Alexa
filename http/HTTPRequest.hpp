#ifndef ALEXA_HTTP_REQUEST
#define ALEXA_HTTP_REQUEST

#ifndef ALEXA_MAX_REQUEST_HEADERS
#define ALEXA_MAX_REQUEST_HEADERS 15
#endif

#ifndef ALEXA_MAX_METHOD_SIZE
#define ALEXA_MAX_METHOD_SIZE 12
#endif

#ifndef ALEXA_MAX_PATH_SIZE
#define ALEXA_MAX_PATH_SIZE 64
#endif

#ifndef ALEXA_MAX_BODY_SIZE
#define ALEXA_MAX_BODY_SIZE 256
#endif

#include "HTTPHeader.hpp"

class HTTPRequest
{
private:
    HTTPHeader headers[ALEXA_MAX_REQUEST_HEADERS];
    int headerCount = 0;

    char *method;//[ALEXA_MAX_METHOD_SIZE];
    char *path;//[ALEXA_MAX_PATH_SIZE];
    char *body;//[ALEXA_MAX_BODY_SIZE];

public:
    HTTPRequest()
    {
        method = (char*)malloc(sizeof(char) * ALEXA_MAX_METHOD_SIZE);
        path = (char*)malloc(sizeof(char) * ALEXA_MAX_PATH_SIZE);
        body = (char*)malloc(sizeof(char) * ALEXA_MAX_BODY_SIZE);

        clear();
    }

    void clear()
    {
        method[0] = '\0';
        path[0] = '\0';
        body[0] = '\0';
    }

    const char *getMethod()
    {
        return method;
    }

    void setMethod(const char *method)
    {
        strncpy(this->method, method, ALEXA_MAX_METHOD_SIZE);
    }

    const char *getPath()
    {
        return path;
    }

    void setPath(const char *path)
    {
        strncpy(this->path, path, ALEXA_MAX_PATH_SIZE);
    }

    const char *getBody()
    {
        return body;
    }

    void setBody(const char *body)
    {
        strncpy(this->body, body, ALEXA_MAX_BODY_SIZE);
    }

    int getHeaderCount()
    {
        return headerCount;
    }

    void addHeader(const char *headerLine)
    {
        if (strstr(headerLine, "HTTP/") == 0)
            return;

        char name[ALEXA_HTTP_HEADER_NAME_SIZE];
        char value[ALEXA_HTTP_HEADER_VALUE_SIZE];
        
        bool valid = false;
        int pos = 0;

        for (int i = 0, n = strlen(headerLine); i < n; i++)
        {
            char c = headerLine[i];
            if (valid)
            {
                value[pos++] = c;
            }
            else if (c == ':')
            {
                valid = true;
                name[pos] = '\0';
                pos = 0;
            }
            else if (!isspace(c))
            {
                name[pos++] = c;
            }
        }
        value[pos] = '\0';

        if (valid)
            setHeader(name, value);
    }

    void setHeader(char *name, char* value)
    {
        if (name == nullptr || value == nullptr)
        {
            Serial.println("NAME OR VALUE NULL!");
            return;
        }

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
};

#endif