#include <Arduino.h>

const int ledPin = 21;       // LED på digital pin 5, så vi döper den till ledPin för att slippa komma ihåg själva siffran + lättare att ändra - magic number
const int buttonPin = 23;    // Tryckknapp på digital pin 2
bool buttonPressed = false;   // För att lagra knappens tillstånd

void setup() {
  pinMode(ledPin, OUTPUT);             // Sätt LED som utgång
  pinMode(buttonPin, INPUT_PULLUP);    // Aktivera intern pull-up motstånd

  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  // Läs in knappens läge, LOW betyder nedtryckt (pga pull-up)
  buttonPressed = (digitalRead(buttonPin) == LOW);

  //Om knappen är nedtryckt, tänd LED. Annars släck
  if (buttonPressed) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(20); // liten debounce - undviker att programmet reagerar på "knycken" i knapptryckningen
}
