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

    ~HTTPHeader()
    {
        //dispose();
    }

    void dispose()
    {
        if (name != nullptr)
            delete name;
        if (value != nullptr)
            delete value;
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