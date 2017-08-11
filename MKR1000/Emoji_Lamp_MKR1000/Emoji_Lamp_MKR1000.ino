#include <TelegramBot.h>
#include <WiFi101.h>
#include <Adafruit_SleepyDog.h>
#include <FastLED.h>

#define NUM_LEDS 30
#define DATA_PIN 7
#define NUM_LEDS_TWO 30
#define DATA_PIN_TWO A5

char ssid[] = "xxxx";             // your network SSID (name)
char pass[] = "xxxx";           // your network key

// Initialize Telegram BOT
const char BotToken[] = "xxxx";

const int BlueButton = 3; 
const int RedButton = 1;
const int YellowButton = 2; 
const int WhiteButton = 4; 

CRGB leds[NUM_LEDS];
CRGB leds_two[NUM_LEDS_TWO];
WiFiSSLClient client;
TelegramBot bot (BotToken, client);

unsigned long Bot_mtbs; //mean time between scan messages
String chatID;
bool lastWhiteButtonStatus=false;

void setup() {
  Serial.begin(115200);
  pinMode(RedButton, INPUT);
  pinMode(BlueButton, INPUT);
  pinMode(YellowButton, INPUT);
  pinMode(WhiteButton, INPUT);
  Serial.println("\n Starting");

  delay(2000);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN_TWO, RGB>(leds_two, NUM_LEDS_TWO);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  notificationGood();

  bot.begin();
  Watchdog.enable(15000);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
  } else {
    Watchdog.reset();
  }
  if (digitalRead(RedButton) == 1) {
    Serial.println("Red");
    Red();
    bot.sendMessage(chatID, "\U00002764"); // heart 
  }
  else if (digitalRead(BlueButton) == 1) {
    Serial.println("Blue");
    Blue();
    bot.sendMessage(chatID, "\U0001F631"); // scream  
  }
  else if (digitalRead(YellowButton) == 1) {
    Serial.println("Yellow");
    Yellow();
    bot.sendMessage(chatID, "\U0001F602"); // joy
  }
  else if (digitalRead(WhiteButton) == 1) {
    Serial.println("White");
    White();
  }


  if (millis() - Bot_mtbs > 2000)  {
    message m = bot.getUpdates(); // Read new messages
    if ( m.chat_id != 0 ) { // Checks if there are some updates
      chatID = m.chat_id;
      Serial.println(m.text);
      if (m.text == "u2764ufe0f" || m.text=="u2764") { // heart emoji
        Red();
      }
      if (m.text == "ud83dude31") { // scream emoji
        Blue();
      }
      if (m.text == "ud83dude02") { // joy emoji
        Yellow();
      }
    }
    Bot_mtbs = millis();
  }
}



void Red() {
  for (int i = 0; i < NUM_LEDS_TWO; i++) {
    leds_two[i] = CRGB(0, 150, 0);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = NUM_LEDS_TWO-1; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}

void Blue() {
  for (int i = 0; i < NUM_LEDS_TWO; i++) {
    leds_two[i] = CRGB(0, 0, 150);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = NUM_LEDS_TWO-1; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}
void Yellow() {
  for (int i = 0; i < NUM_LEDS_TWO; i++) {
    leds_two[i] = CRGB(255, 255, 0);
    FastLED.show();
    delay(50);
  }
  delay(2000);
  for (int i = NUM_LEDS_TWO-1; i >= 0; i--) {
    leds_two[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}

void White() {
  if (lastWhiteButtonStatus == false) {
    lastWhiteButtonStatus = true;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 255, 255);
      FastLED.show();
      delay(50);
    }
  }
  else {
    lastWhiteButtonStatus = false;
    for (int i = NUM_LEDS-1; i >= 0; i--) {
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(50);
    }
  }
}

void notificationGood() {
  for (int a = 0; a < 3; a++) {
    for (int i = 0; i < NUM_LEDS_TWO; i++) {
      leds_two[i] = CRGB(255, 0, 0);
      FastLED.show();
      delay(10);
    }
    for (int i = NUM_LEDS_TWO-1; i >= 0; i--) {
      leds_two[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(10);
    }
  }
}

void notificationReset() {
  for (int a = 0; a < 3; a++) {
    for (int i = 0; i < NUM_LEDS_TWO; i++) {
      leds_two[i] = CRGB(255, 255, 255);
      FastLED.show();
      delay(10);
    }
    for (int i = NUM_LEDS_TWO-1; i >= 0; i--) {
      leds_two[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(10);
    }
  }
}

