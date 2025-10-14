#include <Arduino.h>

const int led1 = 18;
const int led2 = 19;
const int led3 = 21;
const int buttonPin = 23;

int counter = 0;
bool lastButtonState = HIGH;

void showNumber(int value) {
  int led1State, led2State, led3State;

  // Dela upp talet i de tre delarna
  led1State = value % 2;         
  led2State = (value / 2) % 2;   
  led3State = (value / 4) % 2;   

  // Skriv ut till lysdioderna (om ledXstate är sann, tänd LED)
  digitalWrite(led1, led1State ? HIGH : LOW);
  digitalWrite(led2, led2State ? HIGH : LOW);
  digitalWrite(led3, led3State ? HIGH : LOW);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  // Kolla om knappen precis tryckts ner
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    counter++;
    if (counter > 7) 
      counter = 0;
    showNumber(counter); //Uppdatera LED
    delay(200); //Debounce
    }
  }

  lastButtonState = currentButtonState;
}
