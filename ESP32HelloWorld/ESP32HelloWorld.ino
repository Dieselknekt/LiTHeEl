#include <Arduino.h> // Används för Serial och delay
#include <WiFi.h> //används för att ta fram MAC-addressen

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200); //upprättar seriell kommunikation i 115200 baud - symboler per sekund, i praktiken bit/s.
  delay(500); //halv sekunds fördröjning så att allt hinner bli klart innan vi skriver
  Serial.println("\nHello world!");
  Serial.printf("SDK: %s\n", ESP.getSdkVersion());
  Serial.printf("Chip revision: %d\n", ESP.getChipRevision());
  Serial.printf("CPU freq: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());
}

void loop() {

  digitalWrite(2, !digitalRead(2));
  delay(1000);
}

