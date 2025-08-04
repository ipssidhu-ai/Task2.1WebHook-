#include <DHT.h>
#include <WiFiNINA.h>
#include "ThingSpeak.h"

#define DHTPIN 2
#define DHTTYPE DHT11       
#define LEDPIN 4
#define BUZZERPIN 5

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "ishat";      
char pass[] = "ishat001";         
WiFiClient client;

unsigned long channelID = 3025020;             
const char *writeAPIKey = "1IVT1UW2UPGEITQ2"; 

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi Connected!");
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000); 

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C | Humidity: ");
  Serial.print(hum);
  Serial.println("%");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.writeFields(channelID, writeAPIKey);

  // Alarm logic based on temperature
  if (temp > 30) {
    digitalWrite(LEDPIN, HIGH);
    tone(BUZZERPIN, 1500);  
  } else {
    digitalWrite(LEDPIN, LOW);
    noTone(BUZZERPIN);
  }

  delay(15000);  
}

