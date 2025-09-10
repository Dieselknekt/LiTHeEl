#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

//Anslut på 192.168.4.1

#ifndef LED_PIN
#define LED_PIN 2      // justera vid behov (vissa devkort har LED på 5)
#endif

// --- AP-inställningar ---
const char* AP_SSID = "ESP32-LED-ABC";  // gör unikt om du har flera
const char* AP_PASS = "123456789";       // minst 8 tecken, eller "" för öppet nät
const int   AP_CHANNEL = 1;             // 1, 6 eller 11 är bra val
const int   AP_MAX_CONN = 4;            // hur många klienter som får ansluta

WebServer server(80);

// Enkelt webbgränssnitt
String htmlPage(bool ledOn) {
  return String(F(
    "<!doctype html><html><head><meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ESP32 LED</title>"
    "<style>body{font-family:sans-serif;max-width:520px;margin:2rem auto;padding:0 1rem}"
    "button{font-size:1.1rem;padding:.6rem 1rem;margin:.25rem}.on{background:#cfc}"
    ".off{background:#fcc}.state{display:inline-block;min-width:5rem;padding:.2rem .6rem;border:1px solid #ccc;border-radius:.4rem}"
    "</style></head><body>"
    "<h1>ESP32 LED-styrning</h1>"
    "<p>Status: <span id='st' class='state'></span></p>"
    "<p><button class='on'  onclick='req(\"/on\")'>PÅ</button>"
    "<button class='off' onclick='req(\"/off\")'>AV</button>"
    "<button onclick='req(\"/toggle\")'>Växla</button></p>"
    "<script>"
    "async function req(p){await fetch(p);upd();}"
    "async function upd(){try{let r=await fetch('/state');let j=await r.json();"
    "document.getElementById('st').textContent=j.led?'PÅ':'AV';}"
    "catch(e){document.getElementById('st').textContent='okänd';}}"
    "setInterval(upd,1000);window.onload=upd;"
    "</script></body></html>"
  ));
}

// HTTP-handlers
void handleRoot()    { server.send(200, "text/html", htmlPage(digitalRead(LED_PIN))); }
void handleState()   { server.send(200, "application/json", String("{\"led\":") + (digitalRead(LED_PIN) ? "true" : "false") + "}"); }
void handleOn()      { digitalWrite(LED_PIN, HIGH); handleState(); }
void handleOff()     { digitalWrite(LED_PIN, LOW);  handleState(); }
void handleToggle()  { digitalWrite(LED_PIN, !digitalRead(LED_PIN)); handleState(); }
void handleNotFound(){ server.send(404, "text/plain", "Not found"); }

void setup() {
  // LED i definierat läge från start
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  delay(200);
  Serial.println("\nStartar ESP i Access Point-läge...");

  // (valfritt) sätt statisk AP-IP – default är 192.168.4.1 ändå:
  IPAddress ip(192,168,4,1), gw(192,168,4,1), mask(255,255,255,0);
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);                          // jämnare svar (lite mer ström)
  WiFi.softAPConfig(ip, gw, mask);               // måste anropas före softAP()
  WiFi.softAP(AP_SSID, AP_PASS, AP_CHANNEL, false, AP_MAX_CONN);

  Serial.printf("AP igång: SSID=%s  PASS=%s  Kanal=%d\n", AP_SSID, AP_PASS, AP_CHANNEL);
  Serial.printf("Öppna: http://%s/\n", WiFi.softAPIP().toString().c_str());

  // Rutter
  server.on("/",        HTTP_GET, handleRoot);
  server.on("/state",   HTTP_GET, handleState);
  server.on("/on",      HTTP_GET, handleOn);
  server.on("/off",     HTTP_GET, handleOff);
  server.on("/toggle",  HTTP_GET, handleToggle);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Webbserver igång.");
}

void loop() {
  server.handleClient();
}
