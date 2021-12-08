#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "http/HTTPClient.hpp"

#include "EthernetAlexa.h"
byte mac[] = {0xFC, 0xA2, 0xDA, 0xBE, 0xE1, 0x03};
IPAddress addr(192, 168, 1, 117);
IPAddress dns(189, 36, 151, 26);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

HTTPClient client("eu.httpbin.org");

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    pinMode(8, INPUT);
    Serial.println("Serial loaded, waiting for Ethernet...");
    Ethernet.begin(mac, addr, dns, gateway, subnet);
    Serial.println("Ethernet Shield initialized!");
}

int state = LOW;
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
        int readed = digitalRead(8);
        if (readed != state)
            Serial.println(readed, DEC);

        if (readed == HIGH && state != readed)
        {
            HTTPRequest request;
            request.setPath("/");
            request.setMethod("GET");

            HTTPResponse response = client.send(&request);
            Serial.print("Status: ");
            Serial.println(response.getStatusCode(), DEC);
            delay(1000);
        }
        state = readed;
    }
    delay(1);
}