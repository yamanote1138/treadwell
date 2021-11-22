
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
int led = D4;

// instantiate web server
ESP8266WebServer server(80);

// declare web socket server
WebSocketsServer webSocket = WebSocketsServer(81);

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  delay(10);

  // setup onboard LED and turn it off (note: setting it to 'HIGH', turns it off)
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  // File System Init
  SPIFFS.begin();
  
  // connect to wifi
  Serial.printf("\nconnecting to %s\n", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nconnected with IP address of %s\n", WiFi.localIP());
  
  // Start mDNS with name configured hostname
  if (MDNS.begin(mdns_hostname)) Serial.printf("\nmDNS started for %s.local\n", mdns_hostname);

  // configure web server to serve static html/css/img files from SPIFFS root
  server.serveStatic("/", SPIFFS, "/", "max-age=86400");

  // start web server
  server.begin();
  Serial.println("\nweb server started\n");

  webSocket.onEvent(webSocketEventHandler);
  webSocket.begin();
}

void webSocketEventHandler(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("\nClient [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("\nClient [%u] Connected!\n", num);
      break;
    case WStype_TEXT:
      if(payload[0]=='1'){
        digitalWrite(led, LOW);
        Serial.println("LED turned on");
        webSocket.sendTXT(num, "ledOn");
      }else if(payload[0]=='0'){
        digitalWrite(led, HIGH);
        Serial.println("LED turned off");
        webSocket.sendTXT(num, "ledOff");
      }else{
        Serial.printf("\nclient [%u] sent unhandled text: %s\n", num, payload);
      }
      break;
    default:
      break;
  }
}

// the loop function runs over and over again forever
void loop() {
  MDNS.update();
  server.handleClient();
  webSocket.loop();
}
