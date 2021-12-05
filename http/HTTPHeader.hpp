#ifndef ALEXA_HTTP_HEADER
#define ALEXA_HTTP_HEADER

#ifndef ALEXA_HTTP_HEADER_NAME_SIZE
#define ALEXA_HTTP_HEADER_NAME_SIZE 32
#endif

#ifndef ALEXA_HTTP_HEADER_VALUE_SIZE
#define ALEXA_HTTP_HEADER_VALUE_SIZE 128
#endif

#ifndef ALEXA_HTTP_HEADER_SIZE
#define ALEXA_HTTP_HEADER_SIZE ALEXA_HTTP_HEADER_NAME_SIZE + ALEXA_HTTP_HEADER_VALUE_SIZE
#endif

class HTTPHeader
{
private:
    char name[ALEXA_HTTP_HEADER_NAME_SIZE];
    char value[ALEXA_HTTP_HEADER_VALUE_SIZE];

public:
    HTTPHeader()
    {
        clear();
    }

    HTTPHeader(const char *name, const char *value)
    {
        // strncpy(this->name, name, ALEXA_HTTP_HEADER_NAME_SIZE);
        // strncpy(this->value, value, ALEXA_HTTP_HEADER_VALUE_SIZE);
        clear();
    }

    void clear()
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
        //strncpy(this->name, name, ALEXA_HTTP_HEADER_NAME_SIZE);
    }

    const char *getValue()
    {
        return value;
    }

    void setValue(const char *value)
    {
        //strncpy(this->value, value, ALEXA_HTTP_HEADER_VALUE_SIZE);
    }
};

#endif