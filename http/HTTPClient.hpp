#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#ifndef HTTP_MAX_BODY
#define HTTP_MAX_BODY 2048
#endif

#ifndef HTTP_MAX_CALLBACKS
#define HTTP_MAX_CALLBACKS 10
#endif

#include <Ethernet.h>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPClient
{
private:
    const char *host = nullptr;
    IPAddress ip;
    int port;

    char requestBody[HTTP_MAX_BODY];
    uint8_t requestReceived = 0;
    char headerName[HTTP_HEADER_NAME_SIZE];

    bool fetchResponse(EthernetClient *client, HTTPRequest *request, HTTPResponse *response)
    {
        bool name = false, status = false, body = false;
        bool lb = true, version = true;

        requestReceived = 0;
        requestBody[0] = '\0';
        headerName[0] = '\0';

        Serial.println("--------- BEGIN OF RESPONSE ---------");
        while (client->connected() && !client->available())
            delay(1);

        while (client->available())
        {
            char c = client->read();
            Serial.write(c);

            if (version)
            {
                if (isspace(c))
                {
                    requestBody[requestReceived++] = '\0';

                    Serial.print("HTTP Version: ");
                    Serial.println(requestBody);

                    if (strncmp(requestBody, "HTTP/", 5) == 0)
                    {
                        version = false;
                        status = true;

                        requestReceived = 0;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    requestBody[requestReceived++] = c;
                }
            }
            else if (status)
            {
                if (isspace(c))
                {
                    requestBody[requestReceived++] = '\0';

                    Serial.print("Status Received: ");
                    Serial.println(requestBody);
                    int statusCode = atoi(requestBody);

                    response->setStatusCode((HTTPStatusCode)statusCode);

                    if (statusCode == 200)
                    {
                        status = false;
                        name = true;
                        requestReceived = 0;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (!isdigit(c))
                {
                    Serial.print(c);
                    Serial.println(" IS NOT A DIGIT!");
                    return false;
                }
                else
                {
                    requestBody[requestReceived++] = c;
                }
            }
            else if (name)
            {
                if (isspace(c) && requestReceived == 0)
                {
                    continue;
                }
                else if (c == '\r')
                {
                    requestBody[requestReceived++] = '\0';

                    Serial.print("Status Name: ");
                    Serial.println(requestBody);

                    requestReceived = 0;
                    name = false;
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
                if (request->isHeadersRequired())
                {
                }
                else
                {
                }
                requestReceived = 0;
            }
            else if (c == '\n')
            {
                if (lb)
                {
                    body = true;
                }
                else
                {
                    requestBody[requestReceived++] = '\0';
                    Serial.print("Header: ");
                    Serial.println(requestBody);

                    lb = true;
                }
                requestReceived = 0;
            }
            else if (c != '\r')
            {
                Serial.print(c);
                if (requestReceived > 0 || !isspace(c))
                    requestBody[requestReceived++] = c;
                lb = false;
            }
        }
        Serial.println("--------- FINAL OF RESPONSE ---------");

        if (requestReceived > 0)
        {
            requestBody[requestReceived++] = '\0';
            response->setBody(requestBody);
        }

        return true;
    }

public:
    HTTPClient(IPAddress ip, int port = 80)
    {
        this->host = nullptr;
        this->ip = ip;
        this->port = port;
    }

    HTTPClient(const char *host, int port = 80)
    {
        this->host = host;
        this->port = port;
    }

    HTTPResponse send(HTTPRequest *request)
    {
        EthernetClient client;
        HTTPResponse response;
        Serial.print("Connecting to ");

        int connRes = 0;
        if (host == nullptr)
        {
            ip.printTo(Serial);
            Serial.println();
            connRes = client.connect(ip, port);
        }
        else
        {
            Serial.println(host);
            connRes = client.connect(host, port);
        }

        if (connRes == 1)
        {
            Serial.println("Connected, requesting");

            client.print(request->getMethod());
            client.print(" ");
            client.print(request->getPath());
            client.println(" HTTP/1.1");

            client.print("Host: ");
            if (host != nullptr)
            {
                client.println(host);
            }
            else
            {
                ip.printTo(client);
                client.println();
            }

            client.println("Connection: Close");
            client.println();

            fetchResponse(&client, request, &response);
        }
        else
        {
            Serial.print(connRes, DEC);
            Serial.println(" Failed to connect!");
        }

        if (client.connected())
            client.stop();

        return response;
    }
};

#endif