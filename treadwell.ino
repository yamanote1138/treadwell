
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <WebSockets.h>
#include <FS.h>
#include "arduino_secrets.h"

// pull sensitive data from secrets file
char wifi_ssid[] = WIFI_SSID;
char wifi_pass[] = WIFI_PASS;
char mdns_hostname[] = MDNS_HOSTNAME;

// instantiate web server
ESP8266WebServer server(80);

// declare web socket server
WebSocketsServer webSocket = WebSocketsServer(81);

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  delay(10);
  
  // File System Init
  SPIFFS.begin();
  
  // connect to wifi
  Serial.printf("connecting to %s", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("connected with IP address of %s", WiFi.localIP());
  
  // Start mDNS with name configured hostname
  if (MDNS.begin(mdns_hostname)) Serial.printf("mDNS started for %s.local", mdns_hostname);

  // configure web server to serve static html/css/img files from SPIFFS root
  server.serveStatic("/", SPIFFS, "/", "max-age=86400");

  // start web server
  server.begin();
  Serial.println("web server started");

  webSocket.onEvent(webSocketEventHandler);
  webSocket.begin();
}

void webSocketEventHandler(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      
        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // send message back to client
      webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }
}

// the loop function runs over and over again forever
void loop() {
  MDNS.update();
  server.handleClient();
  webSocket.loop();
}
