#ifndef ALEXA_HTTP_SERVER
#define ALEXA_HTTP_SERVER

#ifndef HTTP_MAX_REQUEST
#define HTTP_MAX_REQUEST 150
#endif

#ifndef HTTP_MAX_BODY
#define HTTP_MAX_BODY 1024
#endif

#ifndef HTTP_MAX_CALLBACKS
#define HTTP_MAX_CALLBACKS 10
#endif

#include <Ethernet.h>
#include "HTTPCallback.hpp"

class HTTPServer
{
private:
    EthernetServer *server;
    HTTPCallback *callbacks[HTTP_MAX_CALLBACKS];
    uint8_t callbackCount = 0;

    int port;
    char requestHeader[HTTP_MAX_REQUEST];
    char requestBody[HTTP_MAX_BODY];
    uint8_t requestReceived = 0, bodyReceived = 0;

    const char *getStatusMessage(HTTPStatusCode code)
    {
        switch (code)
        {
        case OK:
            return "200 OK";
        case BadRequest:
            return "400 Bad Request";
        case NotFound:
            return "404 Not Found";
        case NotImplemented:
        default:
            return "501 Not implemented";
        }
    }

public:
    HTTPServer(uint16_t port = 80)
    {
        this->port = port;

        this->server = new EthernetServer(port);
    }

    void begin()
    {
        this->server->begin();
    }

    void loop()
    {
        EthernetClient client = server->available();
        bool lb = true, body = false;

        if (client)
        {
            HTTPRequest request;
            HTTPResponse response;

            requestReceived = 0;
            bodyReceived = 0;

            Serial.print("Client ");
            client.remoteIP().printTo(Serial);
            Serial.println(" connected!");

            //Get the first header
            do
            {
                requestHeader[requestReceived++] = client.read();
            } while (requestHeader[requestReceived - 1] != '\n');
            requestHeader[requestReceived] = '\0';

            while (client.available())
            {
                char c = client.read();

                if (body)
                    requestBody[bodyReceived++] = c;
                else if (c == '\n' && lb)
                    body = true;
                else if (c == '\n')
                    lb = true;
                else if (c != '\r')
                    lb = false;
            }
            requestBody[bodyReceived] = '\0';

            int pos = 0;
            char method[7];
            while (!isspace(requestHeader[pos]))
            {
                method[pos] = requestHeader[pos];
                pos++;
            }
            method[pos] = '\0';

            int methodPos = pos + 1;
            pos = 0;

            char path[56];
            while (!isspace(requestHeader[methodPos + pos]))
            {
                path[pos] = requestHeader[methodPos + pos];
                pos++;
            }
            path[pos] = '\0';

            request.setMethod(method);
            request.setPath(path);
            request.setBody(requestBody);

            Serial.print("Method: ");
            Serial.print(method);
            Serial.print(" - ");
            Serial.println(path);
            Serial.print("BODY: ");
            Serial.println(requestBody);

            bool found = false;
            for (int i = 0; i < callbackCount; i++)
            {
                HTTPCallback *callback = callbacks[i];
                if (callback != nullptr && callback->compare(method, path))
                {
                    found = true;
                    callback->execute(request, &response);
                    break;
                }
            }

            if (!found)
                response.setStatusCode(NotFound);

            const char *statusMessage = getStatusMessage(response.getStatusCode());
            client.print("HTTP/1.1 ");
            client.println(statusMessage);

            for (int i = 0, n = response.getHeaderCount(); i < n; i++)
            {
                HTTPHeader *header = response.getHeader(i);
                client.print(header->getName());
                client.print(": ");
                client.println(header->getValue());
            }
            client.println("Connection: close");

            const char *body = response.getBody();
            if (body == nullptr)
                Serial.println("NULL BODY");
            else if (strlen(body) == 0)
                Serial.println("EMPTY BODY");
            else
                Serial.println(body);

            if (body != nullptr && strlen(body) > 0)
            {
                client.print("Content-Length: ");
                client.println(strlen(body), DEC);

                client.println();
                client.println(body);
            }
            client.println();

            delay(1);
            client.stop();
            Serial.println("Disconnected");
        }
        delay(10);
    }

    int getPort()
    {
        return port;
    }

    void addHttpCallback(HTTPCallback *callback)
    {
        if (callbackCount > HTTP_MAX_CALLBACKS)
            return;

        callbacks[callbackCount++] = callback;
    }
};

#endif