#ifndef ALEXA_HTTP_HEADER
#define ALEXA_HTTP_HEADER

class HTTPHeader
{
private:
    const char *name;
    const char *value;

public:
    HTTPHeader()
    {
    }

    HTTPHeader(const char *name, const char *value)
    {
        this->name = name;
        this->value = value;
    }

    const char *getName()
    {
        return name;
    }

    void setName(const char *name)
    {
        this->name = name;
    }

    const char *getValue()
    {
        return value;
    }

    void setValue(const char *value)
    {
        this->value = value;
    }
};

#endif