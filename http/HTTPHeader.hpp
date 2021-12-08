#ifndef HTTP_HEADER
#define HTTP_HEADER

#include "defines.h"

class HTTPHeader
{
private:
    char name[HTTP_HEADER_NAME_LENGTH];
    char value[HTTP_HEADER_VALUE_LENGTH];

public:
    HTTPHeader()
    {
        name[0] = '\0';
        value[0] = '\0';
    }

    HTTPHeader(const char *name, const char *value)
    {
        strncpy(this->name, name, HTTP_HEADER_NAME_LENGTH);
        strncpy(this->value, value, HTTP_HEADER_VALUE_LENGTH);
    }

    ~HTTPHeader()
    {
        dispose();
    }

    void dispose()
    {
        name[0] = '\0';
        value[0] = '\0';
    }

    const char *getName()
    {
        return name;
    }

    void setName(const char *name)
    {
        char str[sizeof(char) * HTTP_HEADER_NAME_LENGTH];
        strncpy(str, name, strlen(name));

        strncpy(this->name, str, strlen(name));
    }

    const char *getValue()
    {
        return value;
    }

    void setValue(const char *value)
    {
        char str[sizeof(char) * HTTP_HEADER_VALUE_LENGTH];
        strncpy(str, value, HTTP_HEADER_VALUE_LENGTH);

        strncpy(this->value, str, HTTP_HEADER_VALUE_LENGTH);
    }
};

#endif