#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "http/HTTPCallback.hpp"

#include "EthernetAlexa.h"
byte mac[] = {0xFC, 0xA2, 0xDA, 0xBE, 0xE1, 0x03};
IPAddress addr(192, 168, 1, 117);
IPAddress dns(189, 36, 151, 26);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

HTTPServer server;
int relay = LOW;

class HelloWorldCallback : public HTTPCallback
{
    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/";
    }

    bool isHeaderRequired(const char *name)
    {
        return strncmp(name, "Content-Type", 13) == 0;
    }

    void execute(HTTPRequest *request, HTTPResponse *response)
    {
        for (int i = 0, n = request->getHeaderCount(); i < n; i++)
        {
            Serial.print(request->getHeader(i)->getName());
            Serial.print(" ");
            Serial.println(request->getHeader(i)->getValue());
        }
        // char buf[1024];
        // sprintf_P(buf, BODY_HTML, alexa->getDeviceCount(), alexa->isConnected() ? "true" : "false");

        response->setStatusCode(OK);
        response->setHeader("Content-Type", "text/html");
        response->setBody("<h1>oi</h1>");
    }
};

class ToggleRelayCallback : public HTTPCallback
{
    const char *getMethod()
    {
        return "GET";
    }

    const char *getPath()
    {
        return "/toggle";
    }

    void execute(HTTPRequest *request, HTTPResponse *response)
    {
        // char buf[1024];
        // sprintf_P(buf, BODY_HTML, alexa->getDeviceCount(), alexa->isConnected() ? "true" : "false");

        if (relay == LOW)
        {
            relay = HIGH;
        }
        else
        {
            relay = LOW;
        }
        digitalWrite(22, relay);

        response->setStatusCode(OK);
        response->setHeader("Content-Type", "application/json");
        response->setBody("{\"ok\": true}");
    }
};

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    pinMode(22, OUTPUT);
    digitalWrite(22, relay);

    Serial.println("Serial loaded, waiting for Ethernet...");
    Ethernet.begin(mac, addr, dns, gateway, subnet);
    Serial.println("Ethernet Shield initialized!");

    server.addHttpCallback(new HelloWorldCallback());
    server.addHttpCallback(new ToggleRelayCallback());
    server.begin();
}

unsigned char state = LOW;
void loop()
{
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println("No Ethernet Hardware has been found!");
        delay(10000);
    }
    else if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet has been disconnected, trying to reconnect...");
        delay(10000);
    }
    else
    {
        server.loop();
    }
    delay(1);
}