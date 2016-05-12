#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "config.h"

#define SERIALOUT 1

WiFiClient client;
unsigned int last_poll = 0;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html",
  "<html>"
    "<head>"
      "<title>Project Webinterface</title>"
    "</head>"
    "<body>"
      "<p>Aktionen</p>"
      "<ul>"
      "<li><a href='/action1'>Aktion 1</a></li>"
      "<li><a href='/action2'>Aktion 2</a></li>"
      "<li><a href='/getStatus'>Status</a></li>"
      "</ul>"
    "</body>"
  "</html>");
}

void handleAction1() {
  server.send(200, "text/html",
  "<html>"
    "<head>"
      "<title>Project Webinterface</title>"
    "</head>"
    "<body>"
      "<p>Aktion 1 ausgef&uuml;hrt</p>"
      "<ul>"
      "<li><a href='/action1'>Aktion 1</a></li>"
      "<li><a href='/action2'>Aktion 2</a></li>"
      "<li><a href='/getStatus'>Status</a></li>"
      "</ul>"
    "</body>"
  "</html>");
}

void handleAction2() {
  server.send(200, "text/html",
  "<html>"
    "<head>"
      "<title>Project Webinterface</title>"
    "</head>"
    "<body>"
      "<p>Aktion 2 ausgef&uuml;hrt</p>"
      "<ul>"
      "<li><a href='/action1'>Aktion 1</a></li>"
      "<li><a href='/action2'>Aktion 2</a></li>"
      "<li><a href='/getStatus'>Status</a></li>"
      "</ul>"
    "</body>"
  "</html>");
}

void getStatus() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["status"] = "OK";
  root["uptime"] = millis();

  String json_string;
  root.printTo(json_string);
  server.send(200, "application/json", json_string);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  Wire.begin();

  if (SERIALOUT)
    Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (SERIALOUT)
      Serial.print(".");
  }

  if (SERIALOUT) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  server.on("/", handleRoot);
  server.on("/action1", handleAction1);
  server.on("/action2", handleAction2);
  server.on("/getStatus", getStatus);

  server.onNotFound(handleNotFound);

  server.begin();
  if (SERIALOUT)
    Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
  unsigned int now = millis();
  if (last_poll + (1000 * 30) < now) {
    if (SERIALOUT) {
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
  last_poll = now;
  }
  delay(1000);
}
