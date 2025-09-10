#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(500);
  Serial.println("Scannar Wi-Fi...");
}

void loop() {
  int n = WiFi.scanNetworks();
  Serial.printf("\nHittade %d nät:\n", n);
  for (int i = 0; i < n; i++) {
    Serial.printf("%2d: %s  RSSI:%d  %s\n",
                  i + 1,
                  WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i),
                  (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "OPEN" : "SECURED");
  }
  WiFi.scanDelete();
  delay(4000);
}