#ifndef ETHERNET_ALEXA_TEMPLATE
#define ETHERNET_ALEXA_TEMPLATE

//Very thanks for vintlabs for fauxmoESP
//Visit: https://github.com/vintlabs/fauxmoESP

PROGMEM const char HTTP_HEADERS[] = "HTTP/1.1 200 OK\r\n"
                                    "EXT:\r\n"
                                    "HOST: %s:%d"
                                    "CACHE-CONTROL: max-age=100\r\n" // SSDP_INTERVAL
                                    "LOCATION: http://%s:%d/description.xml\r\n"
                                    "SERVER: FreeRTOS/6.0.5, UPnP/1.0, IpBridge/1.17.0\r\n" // _modelName, _modelNumber
                                    "hue-bridgeid: %s\r\n"
                                    "ST: urn:schemas-upnp-org:device:basic:1\r\n"               // _deviceType
                                    "USN: uuid:2f402f80-da50-11e1-9b23-%s::upnp:rootdevice\r\n" // _uuid::_deviceType
                                    "\r\n";

PROGMEM const char DESCRIPTION_XML[] = "<?xml version=\"1.0\" ?>"
                                       "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
                                       "<specVersion><major>1</major><minor>0</minor></specVersion>"
                                       "<URLBase>http://%s:80/</URLBase>"
                                       "<device>"
                                       "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
                                       "<friendlyName>EthernetAlexa (%s:80)</friendlyName>"
                                       "<manufacturer>Royal Philips Electronics</manufacturer>"
                                       "<manufacturerURL>http://www.philips.com</manufacturerURL>"
                                       "<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>"
                                       "<modelName>Philips hue bridge 2012</modelName>"
                                       "<modelNumber>929000226503</modelNumber>"
                                       "<modelURL>http://www.meethue.com</modelURL>"
                                       "<serialNumber>%s</serialNumber>"
                                       "<UDN>uuid:2f402f80-da50-11e1-9b23-%s</UDN>"
                                       "<presentationURL>/</presentationURL>"
                                       "</device>"
                                       "</root>";

PROGMEM const char DEVICE_JSON_TEMPLATE[] = "{"
                                            "\"type\": \"Extended color light\","
                                            "\"name\": \"%s\","
                                            "\"uniqueid\": \"%s\","
                                            "\"modelid\": \"LCT015\","
                                            "\"manufacturername\": \"Philips\","
                                            "\"productname\": \"E4\","
                                            "\"state\":{"
                                            "\"on\": %s,"
                                            "\"bri\": %d,"
                                            "\"xy\": [0,0],"
                                            "\"hue\": 0,"
                                            "\"sat\": 0,"
                                            "\"effect\": \"none\","
                                            "\"colormode\": \"xy\","
                                            "\"ct\": 500,"
                                            "\"mode\": \"homeautomation\","
                                            "\"reachable\": true"
                                            "},"
                                            "\"capabilities\": {"
                                            "\"certified\": false,"
                                            "\"streaming\": {\"renderer\":true,\"proxy\":false}"
                                            "},"
                                            "\"swversion\": \"5.105.0.21169\""
                                            "}";

PROGMEM const char DEVICE_SHORT_JSON_TEMPLATE[] = "{"
                                                  "\"type\": \"Extended color light\","
                                                  "\"name\": \"%s\","
                                                  "\"uniqueid\": \"%s\""
                                                  "\"state\":{"
                                                  "\"on\": %s,"
                                                  "\"mode\": \"homeautomation\","
                                                  "\"reachable\": true"
                                                  "},"
                                                  "}";

#endif