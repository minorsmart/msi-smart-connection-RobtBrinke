#include <Arduino.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPLWrfiOasY"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "-KOpTwS-EsfwkcigmxM8Q2mznnI_2FOu";
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "switchKaKu.h"
char auth[] = "-KOpTwS-EsfwkcigmxM8Q2mznnI_2FOu";
char ssid[] = "WiFi-156";
char pass[] = "0246541pd156";
#define DHTPIN 32
#define DHTTYPE DHT11
#define TRANSMITTERID1 19965
#define rfPin 25
int pumpPin = 33;
const int ldrPin = 39;
int moistSensPin = 36;
int moistValue = 0;
const int ledPin = 2;
int lightValue;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void readDhtSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Successfully read from DHT sensor!");
  Serial.print("The temperature is: ");
  Serial.println(t);
  Serial.print("The humidity is: ");
  Serial.println(h);
  Blynk.virtualWrite(V4, h);
  Blynk.virtualWrite(V5, t);
}

void readLdrPin ()
{
lightValue = analogRead(ldrPin);
float licht = analogRead(ldrPin);
Blynk.virtualWrite(V6, licht);
Serial.println(licht);
}

void checkLight ()
{
  switchKaku(rfPin, TRANSMITTERID1, 1, 1, false, 3);
  delay(0);
readLdrPin();
if (lightValue <1500){
switchKaku(rfPin, TRANSMITTERID1, 1, 1, true, 3);
  delay(20);}
else {
      switchKaku(rfPin, TRANSMITTERID1, 1, 1, false, 3);
  delay(20);}
}

void readMoisture ()
{
moistValue = analogRead(moistSensPin);
Serial.print("Moisture Value = ");
Serial.println(moistValue);
Blynk.virtualWrite(V7, moistValue);
}

void checkSoil ()
{
if (moistValue < 1000){
     Serial.println("Pump is now on");
 digitalWrite(pumpPin, HIGH);
delay(5000);
    Serial.println("Pump is now off");
digitalWrite(pumpPin, LOW);
delay(500);}
}

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  
  dht.begin();

  timer.setInterval(5000L, readDhtSensor);
  timer.setInterval(5000L, readLdrPin);
  timer.setInterval(5000L, readMoisture);
  timer.setInterval(3600000L, checkSoil);
  timer.setInterval(1800000L, checkLight);

pinMode(pumpPin, OUTPUT);
pinMode(ldrPin, INPUT);
pinMode(moistSensPin, INPUT);
pinMode(ldrPin, INPUT);
pinMode(ledPin, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V3)
{
  if (param.asInt()==1) {
    Serial.println("Pump is now on");
 digitalWrite(pumpPin, HIGH);
delay(500);}
else {
    Serial.println("Pump is now off");
digitalWrite(pumpPin, LOW);
delay(500);}
}
