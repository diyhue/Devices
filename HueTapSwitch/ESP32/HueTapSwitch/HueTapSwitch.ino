/*
  Program written by JelleWho, based on DIYHUE ESP8266
  Board: https://dl.espressif.com/dl/package_esp32_index.json
*/

#include <WiFi.h>
#include <ArduinoOTA.h>

const static byte Button[] = {34, 35, 32, 33};  //Where the buttons are (only 0,2,4,12-15,25-27,32-39. can be used
const static byte HttpID[] = {34, 16, 17, 18};  //Http button ID

const char* ssid = "WiFi name"; // replace with your Wi-Fi name
const char* password = "WiFi password"; // replace with your Wi-Fi password
const char* bridgeIp = "192.168.xxx.xxx"; //replace with the hue emulator device IP

//static ip configuration is necessary to minimize bootup time from deep sleep
IPAddress strip_ip   (192, 168,   0,  95); // choose an unique IP Adress
IPAddress gateway_ip (192, 168,   0,   1); // Router IP
IPAddress subnet_mask(255, 255, 255,   0);


//====================
// END USER CONFIG
//====================
#define LED_BUILTIN 2
#define AmountOfButtons sizeof(Button) / sizeof(byte)
bool ButtonState[AmountOfButtons];
const char* switchType = "ZGPSwitch";
byte mac[6];

void setup() {
  for (byte i = 0; i < AmountOfButtons; i++) {  //For each button
    pinMode(Button[i], INPUT);
    ButtonState[i] = digitalRead(Button[i]);       //Save button states to use later
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(strip_ip, gateway_ip, subnet_mask);
  WiFi.macAddress(mac);
  while (WiFi.status() != WL_CONNECTED) {
    delay(25);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   //Let the LED blink to show we are not connected
  }
  digitalWrite(LED_BUILTIN, HIGH);
  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0) {
    WiFiClient client;
    client.connect(bridgeIp, 80);
    String url = "/switch?devicetype=" + (String)switchType + "&mac=" + macToStr(mac);    //register device
    client.connect(bridgeIp, 80);                             //###Registering device
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + bridgeIp + "\r\n" +
                 "Connection: close\r\n\r\n");
  }

  if ((ButtonState[0] == HIGH or digitalRead(Button[0]) == HIGH) and
      (ButtonState[3] == HIGH or digitalRead(Button[3]) == HIGH))   //if button 1&4 are HIGH (Where are are pressed)
  {
    ArduinoOTA.begin();
  } else {
    for (byte i = 0; i < AmountOfButtons; i++) {                  //For each button
      if (ButtonState[i] or digitalRead(Button[i]) == HIGH) {     //if button was or is pressed
        sendHttpRequest(HttpID[i]);                               //Send Http request with appropriate buttonID
        delay(1000);  //My hub doesn't seem to like updates being hammered into it, this delay will just make sure we send more updates a bit later
      }
    }
    Sleep();
  }
}
void loop() {
  ArduinoOTA.handle();

#define EveryXms 500
  static unsigned long LastTime;
  if (millis() > LastTime + EveryXms) {                     //If it's time to update
    LastTime = LastTime + EveryXms;                         //Set new LastTime updated
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   //Blink LED
  }

#define AutoSleepAfterXms 10 * 60 *1000
  if (millis() > AutoSleepAfterXms or                               //If we need to go to auto sleep (OTA requested but not used)
  digitalRead(Button[1]) == HIGH or digitalRead(Button[1]) == HIGH) //If button 2 or 3 is pressed (Maybe user didn't want OTA?)
  {
    Sleep();
  }
}
void sendHttpRequest(int button) {
  WiFiClient client;
  String url = "/switch?mac=" + macToStr(mac) + "&button=" + button;
  client.connect(bridgeIp, 80);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + bridgeIp + "\r\n" +
               "Connection: close\r\n\r\n");
}
void Sleep() {
  yield();
  delay(100);    //Just in case there still things to handle off in the background

  uint64_t Mask = 0xF00000000;

  //  uint32_t MaskTemp = Mask >> 32;
  //  unsigned long long1 = (unsigned long)((MaskTemp & 0xFFFF0000) >> 16 );
  //  unsigned long long2 = (unsigned long)((MaskTemp & 0x0000FFFF));
  //  String hex = String(long1, HEX) + String(long2, HEX); // six octets
  //  Serial.print("mask=");
  //  Serial.print(hex);
  //  Serial.println("_");

  esp_sleep_enable_ext1_wakeup(Mask, ESP_EXT1_WAKEUP_ANY_HIGH); //Set how the module can be woken up
  esp_deep_sleep_start();                             //go right now into sleep mode
}
String macToStr(const byte* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}
