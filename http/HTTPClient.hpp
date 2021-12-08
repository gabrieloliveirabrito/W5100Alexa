#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <Ethernet.h>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPClient
{
private:
    const char *host = nullptr;
    IPAddress ip;
    int port;

    char requestBody[HTTP_MAX_BODY_LENGTH];
    uint16_t requestReceived = 0;
    char headerName[HTTP_HEADER_NAME_LENGTH];

    bool fetchResponse(EthernetClient *client, HTTPRequest *request, HTTPResponse *response)
    {
        bool name = false, status = false, body = false;
        bool lb = true, version = true, header = false;
        bool readBody = true;

        requestReceived = 0;
        requestBody[0] = '\0';
        headerName[0] = '\0';

        DEBUG_PRINTLN("--------- BEGIN OF RESPONSE ---------");
        while (client->connected() && !client->available())
            delay(1);

        while (client->available())
        {
            char c = client->read();

            if (version)
            {
                if (isspace(c))
                {
                    requestBody[requestReceived++] = '\0';

                    DEBUG_PRINT("HTTP Version: ");
                    DEBUG_PRINTLN(requestBody);

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

                    DEBUG_PRINT("Status Received: ");
                    DEBUG_PRINTLN(requestBody);
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
                    DEBUG_PRINT(c);
                    DEBUG_PRINTLN(" IS NOT A DIGIT!");
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
                else if (c == '\n')
                {
                    requestBody[requestReceived++] = '\0';

                    DEBUG_PRINT("Status Name: ");
                    DEBUG_PRINTLN(requestBody);

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
                if (requestReceived % 30 == 0 && requestReceived > 0)
                    DEBUG_PRINTLN();
                else if (requestReceived > 0)
                    DEBUG_PRINT(" ");
                requestBody[requestReceived++] = c;

                if (requestReceived > HTTP_MAX_BODY_LENGTH)
                {
                    DEBUG_PRINTLN("MAX REACHED!");
                    requestBody[requestReceived++] = '\0';
                    break; //Server response huge than buffer
                }
            }
            else if (!header && c == ':')
            {
                requestBody[requestReceived++] = '\0';
                strncpy(headerName, requestBody, requestReceived);
                header = true;

                requestReceived = 0;
            }
            else if (c == '\n')
            {
                if (lb)
                {
                    requestBody[requestReceived++] = '\0';
                    requestReceived = 0;

                    if (request->isBodyRequired())
                    {
                        body = true;
                        DEBUG_PRINTLN("To Body!");
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    requestBody[requestReceived++] = '\0';
                    requestReceived = 0;

                    if (header)
                    {
                        header = false;

                        if (request->isHeadersRequired())
                        {
                            DEBUG_PRINTLN("Appending header");
                            response->setHeader(headerName, requestBody);
                        }

                        // if (strcmp(headerName, "Content-Length") == 0)
                        // {
                        //     request->setBodyRequired(false);
                        // }
                    }

                    lb = true;
                }
            }
            else if (c != '\r')
            {
                if (requestReceived > 0 || !isspace(c))
                    requestBody[requestReceived++] = c;
                lb = false;
            }
        }
        DEBUG_PRINTLN("--------- FINAL OF RESPONSE ---------");

        if (requestReceived > 0 && request->isBodyRequired())
        {
            DEBUG_PRINTLN("Putting the rest of data on body");
            requestBody[requestReceived++] = '\0';
            response->setBody(requestBody);
        }

        return true;
    }

public:
    HTTPClient(IPAddress ip, int port = 80)
    {
#ifdef HTTP_DEBUG_PIN
        pinMode(HTTP_DEBUG_PIN, OUTPUT);
        digitalWrite(HTTP_DEBUG_PIN, LOW);
#endif
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
#ifdef HTTP_DEBUG_PIN
        digitalWrite(HTTP_DEBUG_PIN, HIGH);
#endif

        EthernetClient client;
        HTTPResponse response;
        DEBUG_PRINT("Connecting to ");

        int connRes = 0;
        if (host == nullptr)
        {
#ifdef HTTP_DEBUG
            ip.printTo(Serial);
#endif
            connRes = client.connect(ip, port);
        }
        else
        {
            DEBUG_PRINT(host);
            connRes = client.connect(host, port);
        }
        DEBUG_PRINT("....");

        if (connRes == 1)
        {
            DEBUG_PRINTLN("Connected, requesting");

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
            DEBUG_PRINTLN("Failed to connect to the server!");
        }

        if (client.connected())
            client.stop();

#ifdef HTTP_DEBUG_PIN
        digitalWrite(HTTP_DEBUG_PIN, LOW);
#endif
        return response;
    }
};

#endif