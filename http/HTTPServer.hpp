#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <Ethernet.h>
#include "HTTPCallback.hpp"

class HTTPServer
{
private:
    EthernetServer *server;
    HTTPCallback *callbacks[HTTP_MAX_CALLBACKS];
    uint8_t callbackCount = 0;

    int port;
    char requestBody[HTTP_MAX_BODY_LENGTH];
    uint16_t requestReceived = 0;
    char headerName[HTTP_HEADER_NAME_LENGTH];

    HTTPStatusCode readRequest(EthernetClient *client, HTTPRequest *request, HTTPCallback *&callback)
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
                    DEBUG_PRINTLN("Path found!");
                    path = false;

                    requestBody[requestReceived++] = '\0';
                    request->setPath(requestBody, requestReceived);

                    for (int i = 0; i < callbackCount; i++)
                    {
                        if (callbacks[i] != nullptr && callbacks[i]->compare(*request))
                        {
                            callback = callbacks[i];
                            request->setBodyRequired(callback->isBodyRequired());

                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        DEBUG_PRINTLN("Failed to fetch a callback!");
                        return NotFound;
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
                requestBody[requestReceived++] = c;
            }
            else if (c == ':')
            {
                requestBody[requestReceived++] = '\0';
                if (callback->isHeaderRequired(requestBody))
                {
                    strncpy(headerName, requestBody, requestReceived);
                    headerRequired = true;
                }
                else
                {
                    DEBUG_PRINT("Header ");
                    DEBUG_PRINT(requestBody);
                    DEBUG_PRINTLN(" is not required!");
                }
                requestReceived = 0;
            }
            else if (c == '\n')
            {
                DEBUG_PRINTLN();
                if (lb)
                {
                    if (request->isBodyRequired())
                        body = true;
                    else
                        break;
                }
                else
                {
                    lb = true;

                    if (headerRequired)
                    {
                        headerRequired = false;
                        requestBody[requestReceived++] = '\0';

                        request->setHeader(headerName, requestBody);
                    }
                }
                requestReceived = 0;
            }
            else if (c != '\r')
            {
                if (requestReceived > 0 || !isspace(c))
                {
                    if (requestReceived >= HTTP_HEADER_NAME_LENGTH)
                    {
                        return RequestHeaderFieldsTooLarge;
                    }

                    requestBody[requestReceived++] = c;
                }
                lb = false;
            }
        }

        if (found && requestReceived > 0 && request->isBodyRequired())
        {
            requestBody[requestReceived++] = '\0';
            request->setBody(requestBody, requestReceived);
        }

        return OK;
    }

public:
    HTTPServer(uint16_t port = 80)
    {
        this->port = port;

        this->server = new EthernetServer(port);
    }

    void begin()
    {
#ifdef HTTP_DEBUG_PIN
        pinMode(HTTP_DEBUG_PIN, OUTPUT);
        digitalWrite(HTTP_DEBUG_PIN, LOW);
#endif
        this->server->begin();
    }

    void loop()
    {
        EthernetClient client = server->available();

        if (client)
        {
#ifdef HTTP_DEBUG_PIN
            digitalWrite(HTTP_DEBUG_PIN, HIGH);
#endif
            HTTPRequest request;
            HTTPResponse response(true);
            HTTPCallback *callback;

            requestReceived = 0;
            requestBody[0] = '\0';

            DEBUG_PRINT("Client ");
#ifdef HTTP_DEBUG
            client.remoteIP().printTo(Serial);
#endif
            DEBUG_PRINTLN(" connected!");

            DEBUG_PRINTLN("-------- BEGIN OF REQUEST --------");
            HTTPStatusCode statusCode = readRequest(&client, &request, callback);

            DEBUG_PRINT("Status: ");
            const char *statusMessage = getStatusMessage(response.getStatusCode());
            DEBUG_PRINTLN(statusMessage);

            if (statusCode == OK)
            {
                DEBUG_PRINTLN(request.getMethod());
                DEBUG_PRINTLN(request.getPath());
                DEBUG_PRINTLN(request.getBody());
            }

            if (statusCode == OK && callback != nullptr)
            {
                DEBUG_PRINTLN("Executing Callback!");

                callback->execute(&request, &response);
            }
            else
            {
                response.setStatusCode(statusCode);
            }

            DEBUG_PRINTLN("-------- FINAL OF REQUEST --------");

            client.print("HTTP/1.1 ");
            client.print(response.getStatusCode(), DEC);
            client.print(' ');
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

            DEBUG_PRINTLN("Client disconnected!");
            client.flush();
            client.stop();

            response.dispose();
            request.dispose();

#ifdef HTTP_DEBUG_PIN
            digitalWrite(HTTP_DEBUG_PIN, LOW);
#endif
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