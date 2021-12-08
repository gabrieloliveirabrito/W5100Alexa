#ifndef HTTP_HEADER
#define HTTP_HEADER

#ifndef HTTP_HEADER_NAME_SIZE
#define HTTP_HEADER_NAME_SIZE 32
#endif

#ifndef HTTP_HEADER_VALUE_SIZE
#define HTTP_HEADER_VALUE_SIZE 128
#endif

#ifndef HTTP_HEADER_SIZE
#define HTTP_HEADER_SIZE HTTP_HEADER_NAME_SIZE + HTTP_HEADER_VALUE_SIZE
#endif

class HTTPHeader
{
private:
    char name[HTTP_HEADER_NAME_SIZE];
    char value[HTTP_HEADER_VALUE_SIZE];

public:
    HTTPHeader()
    {
        name[0] = '\0';
        value[0] = '\0';
    }

    HTTPHeader(const char *name, const char *value)
    {
        strncpy(this->name, name, HTTP_HEADER_NAME_SIZE);
        strncpy(this->value, value, HTTP_HEADER_VALUE_SIZE);
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
        char str[sizeof(char) * HTTP_HEADER_NAME_SIZE];
        strncpy(str, name, strlen(name));

        strncpy(this->name, str, strlen(name));
    }

    const char *getValue()
    {
        return value;
    }

    void setValue(const char *value)
    {
        char str[sizeof(char) * HTTP_HEADER_VALUE_SIZE];
        strncpy(str, value, HTTP_HEADER_VALUE_SIZE);

        strncpy(this->value, str, HTTP_HEADER_VALUE_SIZE);
    }
};

#endif