# Manual

Hier ga je lezen hoe je een ledstrip kan koppelen aan een NodeMCU board en aan of uit zet door te praten met een google assistent.

# Benodigdheden

[ifttt.com](http://ifttt.com/) Account

[adafruit.io](http://adafruit.io/) Account

Arduino 2

NodeMCU Board

Ledstrip

![69CBF312-76B8-471C-898F-952C0A61BDF8](https://user-images.githubusercontent.com/58260542/96991210-673f2a00-1528-11eb-92da-809f2cddf912.jpeg)


# Stap 1 De code

We beginnen met het downloaden van een bibliotheek. Die bevat namelijk de basis code die hiervoor nodig is. Het is namenlijk zonde om iets volledig zelf te typen als het al bestaat.

Ga naar Sketch > Include Library > Manage Libraries.

Hier kan je verschillende bibliotheken downloaden. Wij gaan de volgende downloaden genaamd *Adafruit MQTT.*

*Na dit gedaan te hebben kan je naar File > Examples > Adafruit MQTT Library > mqtt_esp8266*

Dit is de basiscode die wij nodig hebben. Hieraan gaan wij alleen nog 1 element aan toevoegen. De code is namenlijk gemaakt om via de serial monitor aan te tonen of de knop aan of uit is gezet. Wij willen dit aan ons ledstrip zien. Hierom voegen wij de volgende code toe:

```
#define PIN        D5  //aan welke pin de ledstrip aangesloten zit

#define NUMPIXELS 30 // Lengte aan ledjes

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

```

Dit toen wij tussen in de void setup tussen de volgende 2 elementen

```
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

  
	******CODE HIER*****

  // Verbinding met onoff feed
  mqtt.subscribe(&onoffbutton);
}

```

Vervolgens moet de code nog weten dat de ledstrip aan moet wanneer de button op "on" is gezet en andersom moet de ledstrip ook uit gaan. Dat doen we door de volgende code:

```
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
```

Hier zeggen we, als de button op "on" staat moet het licht aan, anders moet die uit.

!strcmp wordt gebruikt omdat er 2 strings worden vergeleken. On en Off. degene die een hogere waarde heeft wordt gebruikt.

De code voegen we in de void loop() toe, op de volgende plek:

```
Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2500))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

******CODE HIER******

  Serial.print(F("\\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

```

# Stap 2 Adafruit

Vervolgens maken we een account aan op [Adafruit.io](http://adafruit.io/). Dit is een dashboard dat verschillende componenten aanbiedt die samen kunnen werken met een NodeMCU. Nadat je een account hebt aangemaakt maken we eerst een "Feed". Dit doe je door op "Feed" te drukken en vervolgens op "actions" en dan "Create a new feed". De naam moet "onoff" zijn, de beschrijving mag je leeg laten.

Nu gaan we een Dashboard aanmaken. Dit is het zelfde concept als de Feed. Druk op "Dashboards" en vervolgens op "actions" en dan "Create a new dashboard". De naam voor dit mag je zelf kiezen. Ik noemde het "Licht". Zodra je dit hebt gedaan klik je op je gemaakte dashboard. Nu zie je een zwart scherm met rechts wat buttons. Druk nu op het blauwe vierkantje met het witte plusje en dan op de "Toggle", dat is het eerste vierkantje. Vervolgens koppel je die aan je "onoff" feed. Het volgende scherm heeft geen aanpassing nodig, dit kan je doorklikken. Nu zie je de toggle in je dashboard.

!Laat de tab nog open staan voor de volgende stap!

# Stap 3 Connecten met de code

Nu je een [Adafruit.io](http://adafruit.io/) account hebt moet die nog gekoppeld worden aan je code. Dit doe je door bovenin je code 4 gegevens in te vullen.

De eerste 2 zijn je [adafruit.io](http://adafruit.io/) gegevens. De volgende code is bovenin het bestand van de code te vinden.

```arduino
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "...your AIO username (see <https://accounts.adafruit.com>)..."
#define AIO_KEY         "...your AIO key..."

```

Hier moet je je username invullen tussen de aanhalingstekens, ook moet je een key toevoegen. Dit kan je vinden op de website. Als je nog in je dashboard bent heb je rechts bovenin een waar ook het plusje te vinden was een geel blokje met een sleutel. Als je hier op drukt zie je je gebruikersnaam en AIO Key. Vul die op de aangegeven plek in.

Vervolgens moet de NodeMCU verbinding kunnen maken met het internet om je gegevens te controleren. Hiervoor moet je je WiFI SSID en wachtwoord in de volgende code invullen

```
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...your SSID..."
#define WLAN_PASS       "...your password..."

```

DIt mag ook een hotspot van een telefoon zijn. Zolang het maar verbinding maakt met internet. (Gebruik geen 5Ghz routers, die pakt de NodeMCU niet)

# Stap 4 Voice assistent koppelen

Nu kan je al met de toggle op je dashboard het lampje aan of uitzetten van de NodeMCU. Alleen moeten we nu nog een voice assistent toevoegen. Dit doen we door [ifttt.com](http://ifttt.com/)

Via de website kan je een externe functie toevoegen aan je adafruit, hierdoor kan je bijvoorbeeld zeggen: Zodra ik van huis ben zet het licht uit of zodra ik een afspraak in de agenda heb zet het licht uit.

We moeten 2 applets maken. 1 voor licht uit en 1 voor licht aan. Druk op de grote plus met "create" eronder op de home pagina. Nu moet je voor If This google assistent toevoegen. We willen het laten werken met een simple phrase dus dan kiezen we dat. Nu moet je een commando invoeren waarop de google assistent reageert en eventueel een antwoord. (het moet wel in het engels). Zodra je dit hebt gedaan create je de trigger en maak je een Then That aan. Hiervoor selecteer je adafruit en kies je je gemaakte feed. Doet dit nu nog een keer voor de andere functie en dan ben je klaar! Nu is je google assistent aan je code gekoppeld.

Nu kan je door middel van de google assistent het lampje op je NodeMCU aan of uitzetten!

![Untitled](https://user-images.githubusercontent.com/58260542/96991292-86d65280-1528-11eb-8ebc-876589c93fa9.png)


# Problemen

![Untitled 1](https://user-images.githubusercontent.com/58260542/96991349-981f5f00-1528-11eb-9089-8f972a3d71fb.png)

Ik had een probleem met de code doordat ik geen loop gebruikte. Ik probeer door de volgende code het werkend te krijgen

![Untitled 2](https://user-images.githubusercontent.com/58260542/96991393-a66d7b00-1528-11eb-9a94-b6b8f5320d72.png)

Ik probeerde in eerste instantie simpel de ledstrip aan te krijgen door aan te geven: "als de knop aan is zet dan het licht op groen, zo niet op rood" dit werkte alleen niet om deze manier. Na de foutcode te hebben opgezocht kwam ik erachter dat ik een loop moest gebruiken en hierdoor werkte het wel. 
Door de volgende code was het opgelost:

```arduino
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();  
```

## Voorbeeld:

![4B0098AD-5C2C-4A32-94EE-A71BB9A3DE80](https://user-images.githubusercontent.com/58260542/96991427-b38a6a00-1528-11eb-90ae-ab4b17455a89.jpeg)


![30A1F082-6388-421E-8653-DB13470D1E6C](https://user-images.githubusercontent.com/58260542/96991517-d61c8300-1528-11eb-881b-953a52f0454b.jpeg)


### Bronnen

[https://learn.adafruit.com/pyportal-voice-controlled-smart-switch-and-time-display/google-assistant-setup](https://learn.adafruit.com/pyportal-voice-controlled-smart-switch-and-time-display/google-assistant-setup)

[https://www.instructables.com/Control-Your-Projects-With-Google-Assistant-and-Ad/](https://www.instructables.com/Control-Your-Projects-With-Google-Assistant-and-Ad/)

[http://www.robotc.net/wikiarchive/ARDUINO_328_Functions_Strings#:~:text=strcmp,-short](http://www.robotc.net/wikiarchive/ARDUINO_328_Functions_Strings#:~:text=strcmp,-short) strcmp(void&text=(short) Function compares pString1 with,terminated with a zero char.

[https://github.com/adafruit/Adafruit_NeoPixel/issues/171](https://github.com/adafruit/Adafruit_NeoPixel/issues/171)
