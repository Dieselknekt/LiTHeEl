#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define LED_PIN 2 

// ======= Konfiguration =======
const char* SSID = "EXEMPEL";
const char* PASS = "EXEMPEL";
const char* HOST = "esp32-led";  // försök nå: http://esp32-led.local  //ADDERA UNIK SIFFRA

WebServer server(80);

// Enkel HTML-sida med knappar
String htmlPage(bool ledOn) {
  String s = F(
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
    "async function req(path){await fetch(path);upd();}"
    "async function upd(){try{let r=await fetch('/state');let j=await r.json();"
    "document.getElementById('st').textContent=j.led?'PÅ':'AV';}"
    "catch(e){document.getElementById('st').textContent='okänd';}}"
    "setInterval(upd,1000);window.onload=upd;"
    "</script></body></html>"
  );
  return s;
}

// ======= HTTP-handlers =======
void handleRoot() { server.send(200, "text/html", htmlPage(digitalRead(LED_PIN))); }

void handleState() {
  bool on = digitalRead(LED_PIN);
  server.send(200, "application/json", String("{\"led\":") + (on ? "true" : "false") + "}");
}

void handleOn()     { digitalWrite(LED_PIN, HIGH); handleState(); }
void handleOff()    { digitalWrite(LED_PIN, LOW);  handleState(); }
void handleToggle() { digitalWrite(LED_PIN, !digitalRead(LED_PIN)); handleState(); }

void handleNotFound(){ server.send(404, "text/plain", "Not found"); }

// ======= Wi-Fi helper =======
bool connectWifiSTA(unsigned long timeoutMs = 15000) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.printf("Ansluter till %s", SSID);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < timeoutMs) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  return WiFi.status() == WL_CONNECTED;
}

void startAPFallback() {
  const char* apSsid = "ESP32-LED";
  const char* apPass = "12345678"; // minst 8 tecken
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPass);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("Startade access point: %s  (lösen: %s)\n", apSsid, apPass);
  Serial.printf("Öppna: http://%s/\n", ip.toString().c_str()); // typ 192.168.4.1
}

void setup() {
  // Sätt LED i definierat läge från start
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  delay(300);

  // Försök ansluta till ditt Wi-Fi, annars AP-fallback
  if (connectWifiSTA()) {
    Serial.printf("Ansluten! IP: %s\n", WiFi.localIP().toString().c_str());
    if (MDNS.begin(HOST)) {
      Serial.printf("mDNS aktivt: http://%s.local/\n", HOST);
    } else {
      Serial.println("mDNS kunde inte starta (valfritt).");
    }
  } else {
    Serial.println("Kunde inte ansluta inom tidsgränsen – startar AP-läge.");
    startAPFallback();
  }

  // HTTP-rutter
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
