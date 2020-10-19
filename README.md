# Manual

Hier ga je lezen hoe je een NodeMCU board lampje aan of uit zet door te praten met een google assistent.  

# Benodigdheden

[ifttt.com](http://ifttt.com) Account

[adafruit.io](http://adafruit.io) Account

Arduino 2 

NodeMCU Board

![Manual%200e986271beb34d5ba1f3f085d741e030/13F4B1BA-A81A-4F2C-A5C1-06FD56FA221D.jpeg](Manual%200e986271beb34d5ba1f3f085d741e030/13F4B1BA-A81A-4F2C-A5C1-06FD56FA221D.jpeg)

# Stap 1 De code

We beginnen met het downloaden van een bibliotheek. Die bevat namelijk de basis code die hiervoor nodig is. Het is namenlijk zonde om iets volledig zelf te typen als het  al bestaat.

Ga naar Sketch > Include Library > Manage Libraries. 

Hier kan je verschillende bibliotheken downloaden. Wij gaan de volgende downloaden genaamd *Adafruit MQTT.*

*Na dit gedaan te hebben kan je naar File > Examples > Adafruit MQTT Library > mqtt_esp8266*

Dit is de basiscode die wij nodig hebben. Hieraan gaan wij alleen nog 1 element aan toevoegen. De code is namenlijk gemaakt om via de serial monitor aan te tonen of de knop aan of uit is gezet. Wij willen dit op ons board zien. Hierom voegen wij de volgende code toe:

```arduino
// Led in board toevoegen
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
```

Dit toen wij tussen in de void setup tussen de volgende 2 elementen

```arduino
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

Vervolgens moet de code nog weten dat het lampje aan moet wanneer de button op "on" is gezet en andersom moet het lampje ook uit gaan. Dat doen we door de volgende code:

```arduino
if (!strcmp((char*) onoffbutton.lastread, "ON"))
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        digitalWrite(LED_BUILTIN, HIGH);
      }
```

Hier zeggen we, als de button op "on" staat moet het licht aan, anders moet die uit.

!strcmp wordt gebruikt omdat er 2 strings worden vergeleken. On en Off. degene die een hogere waarde heeft wordt gebruikt.

De code voegen we in de void loop() toe, op de volgende plek:

```arduino
Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2500))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

******CODE HIER******

  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
```

# Stap 2 Adafruit

Vervolgens maken we een account aan op [Adafruit.io](http://adafruit.io). Dit is een dashboard dat verschillende componenten aanbiedt die samen kunnen werken met een NodeMCU. Nadat je een account hebt aangemaakt maken we eerst een "Feed". Dit doe je door op "Feed" te drukken en vervolgens op "actions" en dan "Create a new feed". De naam moet "onoff" zijn, de beschrijving mag je leeg laten.

Nu gaan we een Dashboard aanmaken. Dit is het zelfde concept als de Feed. Druk op "Dashboards" en vervolgens op "actions" en dan "Create a new dashboard". De naam voor dit mag je zelf kiezen. Ik noemde het "Licht". Zodra je dit hebt gedaan klik je op je gemaakte dashboard. Nu zie je een zwart scherm met rechts wat buttons. Druk nu op het blauwe vierkantje met het witte plusje en dan op de "Toggle", dat is het eerste vierkantje. Vervolgens koppel je die aan je "onoff" feed. Het volgende scherm heeft geen aanpassing nodig, dit kan je doorklikken. Nu zie je de toggle in je dashboard. 

!Laat de tab nog open staan voor de volgende stap! 

# Stap 3 Connecten met de code

Nu je een [Adafruit.io](http://adafruit.io) account hebt moet die nog gekoppeld worden aan je code. Dit doe je door bovenin je code 4 gegevens in te vullen.

De eerste 2 zijn je [adafruit.io](http://adafruit.io) gegevens. De volgende code is bovenin het bestand van de code te vinden.

```arduino
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY         "...your AIO key..."
```

Hier moet je je username invullen tussen de aanhalingstekens, ook moet je een key toevoegen. Dit kan je vinden op de website. Als je nog in je dashboard bent heb je rechts bovenin een waar ook het plusje te vinden was een geel blokje met een sleutel. Als je hier op drukt zie je je gebruikersnaam en AIO Key. Vul die op de aangegeven plek in.

Vervolgens moet de NodeMCU verbinding kunnen maken met het internet om je gegevens te controleren. Hiervoor moet je je WiFI SSID en wachtwoord in de volgende code invullen

```arduino
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...your SSID..."
#define WLAN_PASS       "...your password..."
```

DIt mag ook een hotspot van een telefoon zijn. Zolang het maar verbinding maakt met internet. (Gebruik geen 5Ghz routers, die pakt de NodeMCU niet)

# Stap 4 Voice assistent koppelen

Nu kan je al met de toggle op je dashboard het lampje aan of uitzetten van de NodeMCU. Alleen moeten we nu nog een voice assistent toevoegen. Dit doen we door [ifttt.com](http://ifttt.com) 

Via de website kan je een externe functie toevoegen aan je adafruit, hierdoor kan je bijvoorbeeld zeggen: Zodra ik van huis ben zet het licht uit of zodra ik een afspraak in de agenda heb zet het licht uit. 

We moeten 2 applets maken. 1 voor licht uit en 1 voor licht aan.  Druk op de grote plus met "create" eronder op de home pagina. Nu moet je voor If This google assistent toevoegen. We willen het laten werken met een simple phrase dus dan kiezen we dat. Nu moet je een commando invoeren waarop de google assistent reageert en eventueel een antwoord. (het moet wel in het engels). Zodra je dit hebt gedaan create je de trigger en maak je een Then That aan. Hiervoor selecteer je adafruit en kies je je gemaakte feed. Doet dit nu nog een keer voor de andere functie en dan ben je klaar! Nu is je google assistent aan je code gekoppeld.

Nu kan je door middel van de google assistent het lampje op je NodeMCU aan of uitzetten!

### Bronnen

[https://learn.adafruit.com/pyportal-voice-controlled-smart-switch-and-time-display/google-assistant-setup](https://learn.adafruit.com/pyportal-voice-controlled-smart-switch-and-time-display/google-assistant-setup)

[https://www.instructables.com/Control-Your-Projects-With-Google-Assistant-and-Ad/](https://www.instructables.com/Control-Your-Projects-With-Google-Assistant-and-Ad/)

[http://www.robotc.net/wikiarchive/ARDUINO_328_Functions_Strings#:~:text=strcmp,-short strcmp(void&text=(short) Function compares pString1 with,terminated with a zero char](http://www.robotc.net/wikiarchive/ARDUINO_328_Functions_Strings#:~:text=strcmp,-short%20strcmp(void&text=(short)%20Function%20compares%20pString1%20with,terminated%20with%20a%20zero%20char).
