#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>


extern "C" {
#include "user_interface.h"
}


/// BEGIN SETUP SENSOR PARAMETERS ///

#define ssid "WiFi Name"
#define password "WiFi Password"

//Set bridge ip
#define bridgeIp "192.168.0.xxx"

// depending on photoresistor you need to setup this value to trigger dark state when light level in room become low enough
#define lightmultiplier 30

// set the sensor ip address on the network (mandatory!!!)
IPAddress strip_ip ( 192,  168,   0,  97);
IPAddress gateway_ip ( 192,  168,   0,   1);
IPAddress subnet_mask(255, 255, 255,   0);

/// END SETUP SENSOR PARAMETERS ////

bool runOTA, operation;
byte mac[6];
int counter, lightlevel;


void goingToSleep() {
  yield();
  delay(100);
  ESP.deepSleep(0);
  yield();
  delay(100);
}

void blinkLed(uint8_t count, int interval = 500) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(2, LOW);
    delay(interval);
    digitalWrite(2, HIGH);
    delay(interval);
  }
}

String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(4, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(strip_ip, gateway_ip, subnet_mask);
  WiFi.macAddress(mac);
  rst_info *rinfo;
  rinfo = ESP.getResetInfoPtr();

  if ((*rinfo).reason != REASON_DEEP_SLEEP_AWAKE) {
    operation = 1;
  }

  if (digitalRead(4) == LOW) {
    runOTA = true;
    blinkLed(4);
    ArduinoOTA.begin();
  }

}

void loop() {
  if (runOTA) {
    ArduinoOTA.handle();
  } else {
    // wait for WiFi connection
    if ((WiFi.status() == WL_CONNECTED)) {
      HTTPClient http;
      String url = "http://";
      url += bridgeIp;
      url += "/switch";
      url += "?mac=" + macToStr(mac);
      if (operation == 1) {
        //register device
        url += "&devicetype=ZLLPresence";
      } else { //send motion request
        pinMode(14, OUTPUT);
        digitalWrite(14, LOW);
        pinMode(A0, INPUT);
        delay(2);
        int lightlevel = (1024 - analogRead(A0)) * lightmultiplier;
        url += "&presence=true";
        url += "&lightlevel=" + String(lightlevel);
        if (lightlevel < 16000) {
          url += "&dark=true";
        } else {
          url += "&dark=false";
        }
        if (lightlevel > 23000) {
          url += "&daylight=true";
        } else {
          url += "&daylight=false";
        }
      }

      http.begin(url);

      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          goingToSleep();
        } else {
          blinkLed(6);
          goingToSleep();
        }
      } else {
        blinkLed(8);
        goingToSleep();
      }

      http.end();
    }
    delay(100);
    counter++;
    if (counter == 100) {
      blinkLed(10);
      goingToSleep();
    }
  }
}
