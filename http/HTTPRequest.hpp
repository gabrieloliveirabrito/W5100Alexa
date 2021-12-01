#ifndef ALEXA_HTTP_REQUEST
#define ALEXA_HTTP_REQUEST

class HTTPRequest
{
private:
    const char *method;
    const char *path;

public:
    const char *getMethod()
    {
        return method;
    }

    void setMethod(const char *method)
    {
        this->method = method;
    }

    const char *getPath()
    {
        return path;
    }

    void setPath(const char *path)
    {
        this->path = path;
    }
};

#endif