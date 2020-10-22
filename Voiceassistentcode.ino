/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Adafruit_NeoPixel.h>

/************************* WiFi *********************************/

#define WLAN_SSID       "SSID"
#define WLAN_PASS       "PASSWORD"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "USERNAME"
#define AIO_KEY         "KEY"

/************************* Global state *********************************/
WiFiClient client;
// Setup voor MQTT client 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

//feed toevoegen
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Code ************************************/

void MQTT_connect();

#define PIN        D5  //aan welke pin de ledstrip aangesloten zit

#define NUMPIXELS 30 // Lengte pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Licht aan uit voice assistent"));

  // Verbinden met WiFi
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  //Lampjes verbinden


 pixels.begin(); 

  // onoff feed toevoegen
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  
 pixels.clear();
 
  MQTT_connect();
  // loop van 2.5 seconden die kijkt of de toggle is veranderd
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

  // als de toggle op aan staat dan lamp aan, anders uit
     if (!strcmp((char*) onoffbutton.lastread, "ON"))
      {
 for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    pixels.show();   
      }
      }
      else
      {
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();  
      }
      }
}

  //connectie check
void MQTT_connect() {
  int8_t ret;

  // stop als al connected is
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // cconnect geeft 0 terg als connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // 5 seconden delay
       retries--;
       if (retries == 0) {
         
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
