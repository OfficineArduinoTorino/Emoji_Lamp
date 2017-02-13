/*******************************************************************
   This sketch allows you to program an Emoji Lamp

   The emoji-lamp is a smart lamp you can chat with throufh a
   Telegram bot. It will react with different colors depending
   on the emoji you sent. It can also be used with another lamp,
   to make them communicate with each other.

   Hardware:
   Olimex ESP8266-EVB

   Product Link: https://www.olimex.com/Products/IoT/ESP8266-EVB/open-source-hardware
   Instructions on how to program it: https://www.olimex.com/Products/IoT/ESP8266-EVB/open-source-hardware

   Extra hardware:
   Strip led ws2812  x2
   Push buttons      x4
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <TelegramBot.h>
#include <WiFiManager.h>
#include <MQTTClient.h>

MQTTClient client;
WiFiClientSecure net_ssl;
WiFiClient net;

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>

#define NUM_LEDS 15  // modify this depending on the number of leds you have
#define DATA_PIN 0 // pin 11 on the olimex shield
#define NUM_LEDS_TWO 15   // modify this depending on the number of leds you have
#define DATA_PIN_TWO 5 // pin 12 on the olimex shield 

const int BlueButton = 14;
const int RedButton = 12;
const int YellowButton = 13;
const int WhiteButton = 2;

unsigned long timerTrigger;
unsigned long TimerMqtt;
unsigned long Bot_lasttime;
const char* empty_string = "";
String token;
bool lastWhiteButtonStatus = false;

String chatID = "0";
const char* MQTTClient_id = String(random(0xffff), HEX).c_str(); //Generate a random client id
char MQTTBroker_ip[] = "broker.hivemq.com";  //Use a public mqtt broker

CRGB leds[NUM_LEDS];
CRGB leds_two[NUM_LEDS_TWO];
TelegramBot * bot;
WiFiManager wifiManager;


void setup() {
  Serial.begin(115200);
  
  pinMode(RedButton, INPUT_PULLUP);
  pinMode(BlueButton, INPUT_PULLUP);
  pinMode(YellowButton, INPUT_PULLUP);
  pinMode(WhiteButton, INPUT_PULLUP);
  Serial.println("\n Starting");

  delay(2000);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN_TWO, RGB>(leds_two, NUM_LEDS_TWO);

  if (!wifiManager.autoConnect("Your_Emoji_lamp")) {  // "Your_Emoji_lamp" is the name of the network you'll need to connec 
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  notificationGood();
  configure_Bot_and_Broker();
}

void configure_Bot_and_Broker() {

  token = wifiManager.getTokenFromFlash();
  bot = new TelegramBot(token.c_str(), net_ssl);
  Serial.print("Created bot using token: ");
  Serial.println(token);
  bot->begin();      // launch Bot functionalities

  client.begin(MQTTBroker_ip, 1883, net);  // connect to broker
  Serial.println(MQTTBroker_ip);
  Serial.print("connecting to the mqtt broker");

  while (!client.connect(MQTTClient_id)) {
    Serial.print(".");
  }
  Serial.println("\nconnected!");
  client.subscribe(token);
}

void loop() {

  if (digitalRead(WhiteButton) == 1) {
    timerTrigger = millis();
  }

  if ( digitalRead(WhiteButton) == 0 && millis() - timerTrigger > 6000) {
    notificationReset();

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around

    if (!wifiManager.startConfigPortal("Your_Emoji_lamp")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    configure_Bot_and_Broker();
  }

  checkButtons();

  if (millis() -  Bot_lasttime > 3000)  {
    client.loop();
    message m = bot->getUpdates(); // Read new messages
    Serial.print("text: ");
    Serial.println(m.text);
    if ( m.chat_id != "0" && m.chat_id != "" ) {
      chatID = m.chat_id;

      if (m.text == "u2764ufe0f") Red(); //Heart received
      else if (m.text == "ud83dude31") Blue(); //Scream received
      else if (m.text == "ud83dude02") Yellow(); //Joy received
    }
    Bot_lasttime = millis();
  }


}

void checkButtons() {

  if (digitalRead(RedButton) == 0) {
    Red();
    bot->sendMessage(chatID, "\U00002764"); // heart
    TimerMqtt = millis();
    client.publish(token, "\U00002764");
    TimerMqtt = millis();
  }
  else if (digitalRead(BlueButton) == 0) {
    Blue();
    bot->sendMessage(chatID,  "\U0001F631"); // scream
    TimerMqtt = millis();
    client.publish(token, "\U0001F631");
    TimerMqtt = millis();
  }
  else if (digitalRead(YellowButton) == 0) {
    Yellow();
    bot->sendMessage(chatID,  "\U0001F602"); // joy
    TimerMqtt = millis();
    client.publish(token, "\U0001F602");
    TimerMqtt = millis();
  }
  else if (digitalRead(WhiteButton) == 0) {
    White();
  }


}

void Red() {
  for (int i = 0; i < 15; i++) {
    leds_two[i] = CRGB(0, 150, 0);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = 9; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}

void Blue() {
  for (int i = 0; i < 15; i++) {
    leds_two[i] = CRGB(0, 0, 150);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = 9; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}
void Yellow() {
  for (int i = 0; i < 15; i++) {
    leds_two[i] = CRGB(255, 255, 0);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = 9; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}

void White() {
  if (lastWhiteButtonStatus == false) {
    lastWhiteButtonStatus = true;
    for (int i = 0; i < 15; i++) {
      leds[i] = CRGB(255, 255, 255);
      FastLED.show();
      delay(50);
    }
  }
  else {
    lastWhiteButtonStatus = false;
    for (int i = 14; i >= 0; i--) {
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(50);
    }
  }
}

void notificationGood() {
  for (int a = 0; a < 3; a++) {
    for (int i = 0; i < 10; i++) {
      leds_two[i] = CRGB(255, 0, 0);
      FastLED.show();
      delay(10);
    }
    for (int i = 9; i >= 0; i--) {
      leds_two[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(10);
    }
  }
}

void notificationReset() {
  for (int a = 0; a < 3; a++) {
    for (int i = 0; i < 10; i++) {
      leds_two[i] = CRGB(255, 255, 255);
      FastLED.show();
      delay(10);
    }
    for (int i = 9; i >= 0; i--) {
      leds_two[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(10);
    }
  }
}


void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  if (millis() - TimerMqtt > 6000) { // put a delay in order to not reply your own messages
    Serial.println("yolo");
    if (payload == "\U00002764") Red(); //Heart received
    else if (payload == "\U0001F631") Blue(); //Scream received
    else if (payload == "\U0001F602") Yellow(); //Joy received
  }
}
