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
    char requestBody[HTTP_MAX_BODY];
    uint8_t requestReceived = 0;
    char headerName[ALEXA_HTTP_HEADER_NAME_SIZE];

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

    bool readRequest(EthernetClient *client, HTTPRequest *request, HTTPCallback *&callback)
    {
        bool method = true, path = false, body = false;
        bool lb = true, found = false, headerRequired = false;
        headerName[0] = '\0';

        while (client->available())
        {
            char c = client->read();

            if (method)
            {
                if (isspace(c))
                {
                    method = false;
                    path = true;

                    requestBody[requestReceived++] = '\0';
                    request->setMethod(requestBody, requestReceived);

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
                    Serial.println("Path found!");
                    path = false;

                    requestBody[requestReceived++] = '\0';
                    request->setPath(requestBody, requestReceived);

                    for (int i = 0; i < callbackCount; i++)
                    {
                        if (callbacks[i] != nullptr && callbacks[i]->compare(*request))
                        {
                            callback = callbacks[i];
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        Serial.println("Failed to fetch a callback!");
                        return false;
                    }

                    requestReceived = 0;
                }
                else
                {
                    requestBody[requestReceived++] = c;
                }
            }
            else if (body)
            {
                if (found)
                {
                    requestBody[requestReceived++] = c;
                }
            }
            else if (c == ':' && found)
            {
                requestBody[requestReceived++] = '\0';
                if (callback->isHeaderRequired(requestBody))
                {
                    strncpy(headerName, requestBody, requestReceived);
                    headerRequired = true;
                }
                else
                {
                    Serial.print("Header ");
                    Serial.print(requestBody);
                    Serial.println(" is not required!");
                }
                requestReceived = 0;
            }
            else if (c == '\n')
            {
                Serial.println();
                if (lb)
                {
                    body = true;
                }
                else
                {
                    lb = true;

                    if (headerRequired)
                    {
                        headerRequired = false;

                        if (requestReceived > 0)
                        {
                            requestBody[requestReceived++] = '\0';
                            request->setHeader(headerName, requestBody);
                        }
                    }
                }
                requestReceived = 0;
            }
            else if (c != '\r')
            {
                if (requestReceived > 0 || !isspace(c))
                    requestBody[requestReceived++] = c;
                lb = false;
            }
        }

        if (found && requestReceived > 0)
        {
            requestBody[requestReceived++] = '\0';
            request->setBody(requestBody, requestReceived);
        }

        return found;
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

        if (client)
        {
            HTTPRequest request;
            HTTPResponse response;
            HTTPCallback *callback;

            requestReceived = 0;
            requestBody[0] = '\0';

            Serial.print("Client ");
            client.remoteIP().printTo(Serial);
            Serial.println(" connected!");

            Serial.println("-------- BEGIN OF REQUEST --------");
            bool result = readRequest(&client, &request, callback);

            if (result)
            {
                Serial.println(request.getMethod());
                Serial.println(request.getPath());
                Serial.println(request.getBody());
            }
            else
                Serial.println("Not Found!");

            if (result && callback != nullptr)
            {
                Serial.println("Executing Callback!");

                callback->execute(&request, &response);
            }
            else
            {
                response.setStatusCode(NotFound);
            }
            Serial.println("-------- FINAL OF REQUEST --------");

            Serial.println(response.getStatusCode(), DEC);
            const char *statusMessage = getStatusMessage(response.getStatusCode());
            client.print("HTTP/1.1 ");
            client.println(statusMessage);
            client.println("Connection: close");

            for (int i = 0, n = response.getHeaderCount(); i < n; i++)
            {
                HTTPHeader *header = response.getHeader(i);
                client.print(header->getName());
                client.print(": ");
                client.println(header->getValue());
            }

            const char *body = response.getBody();
            if (body != nullptr && strlen(body) > 0)
            {
                client.print("Content-Length: ");
                client.println(strlen(body), DEC);

                client.println();
                client.println(body);
            }
            client.println();

            Serial.println("Client disconnected!");
            client.flush();
            client.stop();

            response.dispose();
            request.dispose();
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