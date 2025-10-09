#include <Arduino.h>

const int ledPin = 5;       // LED på digital pin 5, så vi döper den till ledPin för att slippa komma ihåg själva siffran + lättare att ändra - magic number
const int buttonPin = 2;    // Tryckknapp på digital pin 2
bool buttonState = false;   // För att lagra knappens tillstånd

void setup() {
  pinMode(ledPin, OUTPUT);             // Sätt LED som utgång
  pinMode(buttonPin, INPUT_PULLUP);    // Aktivera intern pull-up motstånd

  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  // Läsa knappen: LOW betyder nedtryckt (pga pull-up)
  buttonState = (digitalRead(buttonPin) == LOW);

  if (buttonState) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(20); // liten debounce - undviker att programmet reagerar på "knycken" i knapptryckningen
}
