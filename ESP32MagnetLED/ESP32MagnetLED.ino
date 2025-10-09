/*
OBS!
FUNGERAR BARA MED VERSION 2.0.17 AV esp 32, Espressif Systems
OBS!
*/

#include <Arduino.h>

const int samples = 200;   // antal prov för att etablera baslinje
const int threshold   = 10;    // känslighet (hur stor diff krävs för utslag)
const int loop_delay  = 50;    // ms mellan mätningarna
int baseline = 0; //utgångsvärde

void setup() {
  pinMode(2, OUTPUT); //sätt pin 2 som en output (det är LEDn och D2)
  digitalWrite(2, LOW);

  Serial.begin(115200); //upprätta seriell kommunikation
  delay(500);

  // etablera sensorns baslinje utan magnet
  long sum = 0; //en variabel för summering av alla mätvärden
  for (int i = 0; i < samples; i++) {
    sum += hallRead(); //addera aktuellt värde till den löpande summan
    delay(2); //kort paus
  }
  baseline = (int)(sum / samples); //När alla mätningar är gjorda, fastställ baslinje genom att dividera summan med antal mätningar
  Serial.printf("Baseline: %d\n", baseline);
  Serial.println("När magneten är nära bör LEDn tändas");
}

void loop() {
  int val   = hallRead(); //Spara det aktuella mätvärdet som variabeln val (value)
  int delta = abs(val - baseline); //Identifiera hur stort avstånd det är mellan baslinjen och val
  bool magnet = (delta >= threshold); //magnet är sann om avståndet är större än det angivna tröskelvärdet

  digitalWrite(2, magnet ? HIGH : LOW); //Sätt LED (och D2) till hög om magnet är sann, annars till låg

  Serial.printf("Val=%d, Delta=%d, Magnet=%s\n", //Skriv ut aktuellt värde, avstånd och bedömning om magneten är där eller inte
              val, delta, magnet ? "JA" : "nej");

  delay(loop_delay); //pausa angiven tid innan nästa körning och mätning
}
