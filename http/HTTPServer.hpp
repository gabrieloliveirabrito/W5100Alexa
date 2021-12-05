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
    HTTPRequest request;
    HTTPResponse response;
    uint8_t callbackCount = 0;

    int port;
    char requestBody[HTTP_MAX_BODY];
    uint8_t requestReceived = 0;

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
        pinMode(7, OUTPUT);
        digitalWrite(7, LOW);
        this->server->begin();
    }

    void loop()
    {
        EthernetClient client = server->available();
        bool lb = true, body = false, method = true, path = false;

        if (client)
        {
            digitalWrite(7, HIGH);

            request.clear();
            response.clear();

            requestReceived = 0;
            requestBody[0] = '\0';

            Serial.print("Client ");
            client.remoteIP().printTo(Serial);
            Serial.println(" connected!");

            Serial.println("-------- BEGIN OF REQUEST --------");
            while (client.available())
            {
                char c = client.read();
                Serial.print(c);

                if (method)
                {
                    if (isspace(c))
                    {
                        method = false;
                        path = true;
                        requestBody[requestReceived] = '\0';

                        request.setMethod(requestBody);
                        requestReceived = 0;
                    }
                    else
                    {
                        requestBody[requestReceived++] = c;
                    }
                }
                else if (path)
                {
                    if (isspace(c))
                    {
                        path = false;
                        requestBody[requestReceived] = '\0';

                        request.setPath(requestBody);
                        requestReceived = 0;
                    }
                    else
                    {
                        requestBody[requestReceived++] = c;
                    }
                }
                else if (body)
                {
                    requestBody[requestReceived++] = c;
                }
                else if (c == '\n')
                {
                    if (lb)
                    {
                        body = true;
                    }
                    else
                    {
                        lb = true;

                        requestBody[requestReceived] = '\0';
                        request.addHeader(requestBody);
                    }
                    requestReceived = 0;
                }
                else if (c != '\r')
                {
                    requestBody[requestReceived++] = c;
                    lb = false;
                }
            }
            Serial.println("-------- FINAL OF REQUEST --------");

            bool found = false;
            for (int i = 0; i < callbackCount; i++)
            {
                HTTPCallback *callback = callbacks[i];
                if (callback != nullptr && callback->compare(request))
                {
                    //     Serial.println("Found!");
                    //     found = true;
                    //     //callback->execute(request, response);

                    //     Serial.println(response.getStatusCode(), DEC);
                    break;
                }
            }

            if (!found)
                response.setStatusCode(NotFound);

            const char *statusMessage = getStatusMessage(response.getStatusCode());
            client.print("HTTP/1.1 ");
            client.println(statusMessage);

            Serial.println(response.getHeaderCount(), DEC);
            for (int i = 0, n = response.getHeaderCount(); i < n; i++)
            {
                HTTPHeader *header = response.getHeader(i);
                client.print(header->getName());
                client.print(": ");
                client.println(header->getValue());
            }
            client.println("Connection: close");

            const char *body = response.getBody();
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
            digitalWrite(7, LOW);
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