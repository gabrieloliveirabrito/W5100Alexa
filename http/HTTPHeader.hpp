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
        name[0] = '\0';
        value[0] = '\0';
    }

    HTTPHeader(const char *name, const char *value)
    {
        strncpy(this->name, name, ALEXA_HTTP_HEADER_NAME_SIZE);
        strncpy(this->value, value, ALEXA_HTTP_HEADER_VALUE_SIZE);
    }

    ~HTTPHeader()
    {
        //Serial.println("Disposing HTTPHeader");
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
        char str[sizeof(char) * ALEXA_HTTP_HEADER_NAME_SIZE];
        strncpy(str, name, strlen(name));

        strncpy(this->name, str, strlen(name));
    }

    const char *getValue()
    {
        return value;
    }

    void setValue(const char *value)
    {
        char str[sizeof(char) * ALEXA_HTTP_HEADER_VALUE_SIZE];
        strncpy(str, value, ALEXA_HTTP_HEADER_VALUE_SIZE);

        strncpy(this->value, str, ALEXA_HTTP_HEADER_VALUE_SIZE);
    }
};

#endif