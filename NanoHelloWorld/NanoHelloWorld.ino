#include <Arduino.h> // Används för Serial och delay

void setup() {
  pinMode(13, OUTPUT);               // Sätt pin 13 (onboard LED) till OUTPUT
  Serial.begin(9600);                // upprättar seriell kommunikation i 9600 baud - symboler per sekund, i praktiken bit/s.
  delay(500);                        // halv sekunds fördröjning så att allt hinner bli klart innan vi skriver
  Serial.println("Hello world!");
}

void loop() {
  digitalWrite(13, !digitalRead(13));  // Växla LED på/av
  delay(1000);
}
