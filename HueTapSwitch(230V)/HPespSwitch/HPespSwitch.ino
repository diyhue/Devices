//  Hue 4x Switch by Hunger Philipp
//  rev 0.1 initial Release
//  rev 0.2 bugfix Switchname
//  rev 0.3 correct GPIO nummbers

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

extern "C" {
#include "gpio.h"
#include "user_interface.h"
}

const char* ssid = "****";         
const char* password = "****";


//connect the buttons to the GPIOs
#define button1_pin 12
#define button2_pin 13
#define button3_pin 5
#define button4_pin 4

bool btn1_trig = false;
bool btn2_trig = false;
bool btn3_trig = false;
bool btn4_trig = false;


const char* switchType = "ZGPSwitch";
const char* otaName = "Wohnzimmer Tap-Switch";  // Name for OverTheAir Updates
const char* bridgeIp = "192.168.2.100";         // IP of the Bridge

//Static adresses are no longer needed, because we use a Powersupply!
//DHCP FTW :)

//#define USE_STATIC_IP //! uncomment to enable Static IP Adress
#ifdef USE_STATIC_IP
IPAddress strip_ip ( 192,  168,   0,  95); // choose an unique IP Adress
IPAddress gateway_ip ( 192,  168,   0,   1); // Router IP
IPAddress subnet_mask(255, 255, 255,   0);
#endif

int counter;
byte mac[6];


String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void sendHttpRequest(int button) {
  WiFiClient client;
  String url = "/switch?mac=" + macToStr(mac) + "&button=" + button;
  client.connect(bridgeIp, 80);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + bridgeIp + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + bridgeIp + "\r\n" +
                 "Connection: close\r\n\r\n");
}


void ISR_S1() {
  for (int i = 0; i < 5000; i++)
  {
    _NOP();
  }

  btn1_trig = true;
  
}

void ISR_S2() {

  for (int i = 0; i < 5000; i++)
  {
    _NOP();
  }

  btn2_trig = true;
  
}

void ISR_S3() {

  for (int i = 0; i < 5000; i++)
  {
    _NOP();
  }

  btn3_trig = true;
  
}

void ISR_S4() {

  for (int i = 0; i < 5000; i++)
  {
    _NOP();
  }

  btn4_trig = true;
  
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup!");

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  pinMode(16, OUTPUT);
  
  pinMode(button1_pin, INPUT);
  pinMode(button2_pin, INPUT);
  pinMode(button3_pin, INPUT);
  pinMode(button4_pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(button1_pin), ISR_S1, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2_pin), ISR_S2, FALLING);
  attachInterrupt(digitalPinToInterrupt(button3_pin), ISR_S3, FALLING);
  attachInterrupt(digitalPinToInterrupt(button4_pin), ISR_S4, FALLING);

  digitalWrite(16, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
#ifdef USE_STATIC_IP
  WiFi.config(strip_ip, gateway_ip, subnet_mask);
#endif

  WiFi.macAddress(mac);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  Serial.println("Connected");
  
//  ArduinoOTA.setPort(8266);  //8266 is the Default Port
//  Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(otaName);
  ArduinoOTA.setPasswordHash("otapassword"); // or MD5 Hash https://www.md5-generator.de/

  ArduinoOTA.begin();

  rst_info *rinfo;
  rinfo = ESP.getResetInfoPtr();

  if ((*rinfo).reason != REASON_DEEP_SLEEP_AWAKE) {

    WiFiClient client;
    client.connect(bridgeIp, 80);

    //register device
    String url = "/switch";
    url += "?devicetype=" + (String)switchType;
    url += "&mac=" + macToStr(mac);

    //###Registering device
    client.connect(bridgeIp, 80);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + bridgeIp + "\r\n" +
                 "Connection: close\r\n\r\n");
}
}

void loop() {
  ArduinoOTA.handle();
  delay(1);

  //Serial.println("read...");
  delay(10);


  if (btn1_trig == true)  
  {
    btn1_trig = false;
      sendHttpRequest(34);
}

if (btn2_trig == true)
  {
    btn2_trig = false;
      sendHttpRequest(16);
}

if (btn3_trig == true)
  {
    btn3_trig = false;
      sendHttpRequest(17);
}

if (btn4_trig == true)
  {
    btn4_trig = false;
      sendHttpRequest(18);
}  
  delay(500);  // delay for Data transmission to the Bridge
}
