
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

// pin aliases
int led = D4;
int pwmA = D1;
int aIn1 = D2;
int aIn2 = D3;
int pwmB = D5;
int bIn1 = D6;
int bIn2 = D7;

// motor defaults
int speed = 128; // ~50%
int duration = 1000; // 1 second

// instantiate web server
ESP8266WebServer server(80);

// declare web socket server
WebSocketsServer webSocket = WebSocketsServer(81);

void move(int l=0, int r=0){
  analogWrite(pwmA, 0);
  switch(l){
    case 0: //stop
      digitalWrite(aIn1, LOW);
      digitalWrite(aIn2, LOW);
      analogWrite(pwmA, 0);
      break;
    case 1: //fwd
      digitalWrite(aIn1, HIGH);
      digitalWrite(aIn2, LOW);
      analogWrite(pwmA, speed);
      break;
    case 2: //reverse
      digitalWrite(aIn1, LOW);
      digitalWrite(aIn2, HIGH);
      analogWrite(pwmA, speed);
      break;
    default:
      Serial.printf("unhandled left motor value: %u\n", l);
  }
  switch(r){
    case 0: //stop
      digitalWrite(bIn1, LOW);
      digitalWrite(bIn2, LOW);
      analogWrite(pwmB, 0);
      break;
    case 1: //fwd
      digitalWrite(bIn1, HIGH);
      digitalWrite(bIn2, LOW);
      analogWrite(pwmB, speed);
      break;
    case 2: //reverse
      digitalWrite(bIn1, LOW);
      digitalWrite(bIn2, HIGH);
      analogWrite(pwmB, speed);
      break;
    default:
      Serial.printf("unhandled right motor value: %u\n", r);
  }
}

void stop(){
  move(0,0);
}

void forward(){
  stop();
  move(1,1);
  delay(duration);
  stop();
}

void backward(){
  stop();
  move(2,2);
  delay(duration);
  stop();
}

void left(){
  stop();
  move(2,1);
  delay(duration);
  stop();
}

void right(){
  stop();
  move(1,2);
  delay(duration);
  stop();
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  delay(10);

  // setup onboard LED and turn it off (note: setting it to 'HIGH', turns it off)
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  // setup motor pins
  pinMode(pwmA, OUTPUT);
  pinMode(aIn1, OUTPUT);
  pinMode(aIn2, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(bIn1, OUTPUT);
  pinMode(bIn2, OUTPUT);

  // set motor pins to an idle state
  stop();

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
      }else if(payload[0]=='F'){
        forward();
        Serial.println("moved forward");
        webSocket.sendTXT(num, "F");
      }else if(payload[0]=='B'){
        backward();
        Serial.println("moved backward");
        webSocket.sendTXT(num, "B");
      }else if(payload[0]=='L'){
        left();
        Serial.println("moved left");
        webSocket.sendTXT(num, "L");
      }else if(payload[0]=='R'){
        right();
        Serial.println("moved right");
        webSocket.sendTXT(num, "R");
      }else if(payload[0]=='S'){
        stop();
        Serial.println("all stop");
        webSocket.sendTXT(num, "S");
      }else {
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
