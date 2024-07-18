
const char* ssid = "";
const char* password = "";
unsigned long flickerInterval = 20; // Initial flicker interval

void initWiFi() {
  unsigned long startAttemptTime = millis();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  // Attempt to connect for 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    if (millis() - lastUpdate >= 20) {
      lastUpdate = millis();
      ws2812b.clear();                                              // Clear all LEDs
      ws2812b.setPixelColor(currentLED, ws2812b.Color(255, 0, 0));  // Set the current LED to red
      ws2812b.show();                                               // Update the strip
      currentLED = (currentLED + 1) % NUM_PIXELS;                   // Move to the next LED
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
    Serial.print("Status: ");
    Serial.println(WiFi.status());
    ESP.restart();
  }
}