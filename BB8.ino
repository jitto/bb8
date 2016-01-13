#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "webpages.h"

ESP8266WebServer server(80);

#define ESP8266_LED 5

void handleRoot() {
	server.send(200, "text/html", index_html);
  Serial.println("Served /");
}

void handleRover() {
  String power = server.arg("power");
  String direction = server.arg("direction");
  Serial.println("Changing power to " + power + " and direction to " + direction);
}

void handleAcceleration() {
  String acceleration = server.arg("value");
  Serial.println("Changing acceleration to " + acceleration);
}

void initWiFi() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  String ssid = "BB8-" + String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  ssid.toUpperCase();
  char* ssidChar = new char[ssid.length()];
  ssid.toCharArray(ssidChar, ssid.length());
  Serial.print("SSID: ");
  Serial.println(ssidChar);
    
  WiFi.softAP(ssidChar);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  WiFi.printDiag(Serial);
  server.on("/", handleRoot);
  server.on("/rover", handleRover);
  server.on("/acceleration", handleAcceleration);
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
	delay(10000);
  initWiFi();
  pinMode(ESP8266_LED, OUTPUT);
}

void loop() {
	server.handleClient();
}
