#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>


IPAddress server(192, 168, 0, 56); // IP de la raspberry Pi
const char* ssid     = "SSID"; // Your ssid
const char* pass = "PASSWORD"; // Your Password
const char* host = "Sonoff1estado"; // nombre del entorno

int rele = 12;
int led = 13;
int pulsador = 0;
boolean estado_pulsador ;
boolean estado_luz = 0;
#define BUFFER_SIZE 100



WiFiClient wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& pub) {
  Serial.println (pub.payload_string());
    if(pub.payload_string() == "on")
    {
      digitalWrite(rele, HIGH); // en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
      digitalWrite(led, LOW);
      estado_luz=1;
      Serial.println("Switch On");
          
    }
    else
    {
      digitalWrite(rele, LOW); // en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
      digitalWrite(led, HIGH);
      estado_luz=0;
      Serial.println("Switch Off");
    }

    EEPROM.write(0,estado_luz);
    EEPROM.commit();
}

void setup() 
{
  pinMode(rele,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(pulsador,INPUT);
  delay(1000);
  digitalWrite(led, LOW);
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();
  client.set_callback(callback);
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
    Serial.println("WiFi not connected");
    }
    else
    {
    Serial.println("WiFi connected");
    for (int i=0; i<10; i++)
    {
    digitalWrite(led, LOW);
    delay(150);
    digitalWrite(led, HIGH);
    delay(150);
    }                         
    }
  }
  estado_luz = EEPROM.read(0);
  if (estado_luz ==  0)
  {
    digitalWrite(rele, LOW);// en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(rele, HIGH);// en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
    digitalWrite(led, LOW);
  }
  delay(200);
}

void loop() {


  estado_pulsador=digitalRead(pulsador);
  if (estado_pulsador==LOW)
  {
    if (estado_luz==0)
    {
      estado_luz=1;
      client.publish("Sonoff1estado","SwitchedOn");
      Serial.println("Switch On");
    }
    else
    {
      estado_luz=0; 
      client.publish("Sonoff1estado","SwitchedOff");
      Serial.println("Switch Off");
    }

    delay(250);
  }

    if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("ESP8266: Sonoff1estado")) {
        client.publish("outTopic",(String)"hello world, I'm "+host);
        client.subscribe(host+(String)"/#");
      }
    }
    if (client.connected())

      client.loop();
  }
  else
  {
    if (estado_luz==0)
    {
       digitalWrite(rele,HIGH); // en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
       digitalWrite(led, LOW);
    }
    else
    {
      digitalWrite(rele,LOW);  // en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
      digitalWrite(led, HIGH);
    }    
    EEPROM.write(0,estado_luz);
    EEPROM.commit();
    delay(250);
  }
 
}



