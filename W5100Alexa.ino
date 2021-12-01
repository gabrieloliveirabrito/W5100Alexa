#ifndef Arduino_h
#include <Arduino.h>
#endif

#include "EthernetAlexa.h"
byte mac[] = {0xFC, 0xA2, 0xDA, 0xBE, 0xE1, 0x03};
IPAddress addr(192, 168, 1, 117);
IPAddress dns(189, 36, 151, 26);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

AlexaLightDevice yellowLed("Yellow Led");

EthernetAlexa alexa;
void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("Serial loaded, waiting for Ethernet...");
    Ethernet.begin(mac, addr, dns, gateway, subnet);
    Serial.println("Ethernet Shield initialized!");

    alexa.addDevice(&yellowLed);
    //alexa.setDiscoverable(false);

    pinMode(21, OUTPUT);
}

unsigned char state = LOW;
void loop()
{
    if (!alexa.isConnected())
    {
        Serial.println("Waiting for Alexa...");
        while (1)
        {
            if (alexa.begin())
            {
                Serial.println("Alexa connected successfully!");
                break;
            }
            else
            {
                Serial.println("Failed to connect to Alexa!");
                delay(10000);
            }
        }
    }
    else
    {
        alexa.loop();
    }
    delay(1);
}