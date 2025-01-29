/*
  * Quellcode von:
  * https://polluxlabs.net/esp8266-projekte/die-aktuelle-uhrzeit-mit-einem-esp8266-abfragen/
  * abgerufen am 23.11.2023
  */

#include <ESP8266WiFi.h>  // libraries for wifi
#include <NTPClient.h>     // time lib
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>  // Lib for LEDS

// Define the Pins for the buttons, LED_size and LED
#define LED_PIN D1 
#define BUTTON_RED_PIN D2 
#define BUTTON_BLACK_PIN D3 
#define STRIPSIZE 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "FRITZ!Box 7530 YX";
const char *password = "17858412786313066094";

// Zeitverschiebung UTC <-> MEZ (Winterzeit) = 3600 Sekunden (1 Stunde)
// Zeitverschiebung UTC <-> MEZ (Sommerzeit) = 7200 Sekunden (2 Stunden)
const long utcOffsetInSeconds = 3600;

int count = 0;  // Loop counter
int timer = 0;  // for the multiple delays
bool Start = true;  // for the start button
unsigned long startTime = 0;
const unsigned long waitDuration_5min = 300;
const unsigned long waitDuration_2min = 120;

char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

void rainbow(uint8_t wait) {  // rainbow function
  uint16_t i, j;
  
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i * 1 + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {  // needed for the rainbow function
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);  // Initialize Red Button
  pinMode(BUTTON_BLACK_PIN, INPUT_PULLUP);  // Initialize Black Button

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  // Wifi status unconnected
    delay(1000);
    Serial.println("Ich verbinde mich mit dem Internet...");
  }
  
  Serial.println("Ich bin mit dem Internet verbunden!");

  timeClient.begin();
  timeClient.update();

  strip.begin();  // Only works if board is connected to Internet
  strip.setBrightness(50);
  strip.show();  // Initialize all pixels to off
}

void loop() {
  if (!digitalRead(BUTTON_RED_PIN) && !digitalRead(BUTTON_BLACK_PIN) && Start) {  // start can only be pressed once
    Start = false;
  }

  while (!Start && count < 4) {  // Start

    timeClient.update();

    long currentTime = timeClient.getEpochTime();
    long startTime = timeClient.getEpochTime();

    for (int i = 0; i <= count; i++) {  // Turn Loop LED on
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show();
    }
    
    if (count > 0) {
      while (currentTime - startTime < waitDuration_5min - 1) {  // 5min using realtime as reference
        currentTime = timeClient.getEpochTime();
        delay(1000);
      }
    } else {  // first loop starts faster; the rest has a 5min wait time in between
      delay(10000);
    }

    for (int i = 0; i <= STRIPSIZE; i++) {  // Turn LED off
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }

    for (int i = 0; i <= STRIPSIZE; i++) {  // 2min counter for LED
      currentTime = timeClient.getEpochTime();
      startTime = timeClient.getEpochTime();

      strip.setPixelColor(i, strip.Color(255, 255, 255));  // white
      strip.show();

      while (currentTime - startTime < waitDuration_2min - 1) {  // 2min using realtime as reference
        currentTime = timeClient.getEpochTime();
        delay(1000);
      }
    }

    for (int i = 0; i <= STRIPSIZE; i++) {  // Turn LEDS off
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    
    count++;

    if (count == 4) {  // After last loop
      Start = true;  // Resets the function and enables the start button again.
      count = 0;  // resets the count
      rainbow(30);
      for (int i = 0; i <= STRIPSIZE; i++) {  // turn LED off
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
      }
    }

    timeClient.update();
  }
}
