#ifndef ALEXA_HTTP_REQUEST
#define ALEXA_HTTP_REQUEST

class HTTPRequest
{
private:
    const char *method;
    const char *path;
    const char *body;

public:
    ~HTTPRequest()
    {
        dispose();
    }

    void dispose()
    {
        if (method != nullptr)
            delete method;
        if (path != nullptr)
            delete path;
        if (body != nullptr)
            delete body;
    }

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

    const char *getBody()
    {
        return body;
    }

    void setBody(const char *body)
    {
        this->body = body;
    }
};

#endif