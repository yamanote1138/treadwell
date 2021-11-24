
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <WebSockets.h>
#include <FS.h>
#include "arduino_secrets.h"
// #include "Motor.h"
#include "Tank.h"

// pull sensitive data from secrets file
char wifi_ssid[] = WIFI_SSID;
char wifi_pass[] = WIFI_PASS;
char mdns_hostname[] = MDNS_HOSTNAME;

// pin aliases
int pwmA = D1;
int aIn1 = D2;
int aIn2 = D3;
int stby = D4;
int pwmB = D5;
int bIn1 = D6;
int bIn2 = D7;

// motor defaults
int speed = 128; // ~50%
int duration = 1000; // 1 second

ESP8266WebServer server(80); // instantiate web server
WebSocketsServer socket(81); // instantiate web socket server
Tank treadwell(pwmA, aIn1, aIn2, stby, pwmB, bIn1, bIn2); // instantiate tank robot

void initWifi(){
  Serial.printf("\nconnecting to %s\n", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nconnected with IP address of %s\n", WiFi.localIP());
}

void initMDNS(){
  // Start mDNS with name configured hostname
  if (MDNS.begin(mdns_hostname)) Serial.printf("\nmDNS started for %s.local\n", mdns_hostname);
}

void initWebServer(){
  // configure web server to serve static html/css/img files from SPIFFS root
  server.serveStatic("/", SPIFFS, "/", "max-age=86400");

  // start web server
  server.begin();
  Serial.println("\nweb server started\n");
}

void initWebSocketServer(){
  socket.onEvent(webSocketEventHandler);
  socket.begin();
}

void processWebSocketMessage(uint8_t num, uint8_t * payload){
  if(payload[0]=='F'){
    treadwell.forward(speed, duration);
    Serial.println("moved forward");
    socket.sendTXT(num, "F");
  }else if(payload[0]=='B'){
    treadwell.reverse(speed, duration);
    Serial.println("moved backward");
    socket.sendTXT(num, "B");
  }else if(payload[0]=='L'){
    treadwell.left(speed, duration);
    Serial.println("moved left");
    socket.sendTXT(num, "L");
  }else if(payload[0]=='R'){
    treadwell.right(speed, duration);
    Serial.println("moved right");
    socket.sendTXT(num, "R");
  }else if(payload[0]=='S'){
    treadwell.stop();
    Serial.println("all stop");
    socket.sendTXT(num, "S");
  }else {
    Serial.printf("\nclient [%u] sent unhandled text: %s\n", num, payload);
  }
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
      processWebSocketMessage(num, payload);
      break;
    default:
      break;
  }
}

void setup() {
  
  Serial.begin(115200);
  delay(10);

  treadwell.stop(); // set motor pins to an idle state

  SPIFFS.begin(); // initialize SPIFFS file system
  
  initWifi(); // connect to wifi
  
  initMDNS();  // set up mDNS

  initWebServer(); // set up webserver

  initWebSocketServer();
}

// the loop function runs over and over again forever
void loop() {
  MDNS.update(); // keep mdns correlation up to date
  server.handleClient(); // watch for web requests
  socket.loop(); // keep socket alive and handle messages
}
