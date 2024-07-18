const char* host = "192.168.2.9";
const uint16_t port = 12345;


void initTCPClient() {
  if (client.connect(host, port)) {
    Serial.println("Connected to server");
    setColor(2);
    ws2812b.show();
  } else {
    Serial.println("Connection failed.");
    setColor(5);
    ws2812b.show();
  }
}
void handleClient() {

  if (!client.connected()) {
    Serial.println("Lost server connection. Reconnecting...");
    if (millis() - lastUpdateClient >= 50) {
      lastUpdateClient = millis();
      ws2812b.clear();                                                // Clear all LEDs
      ws2812b.setPixelColor(currentLED, ws2812b.Color(255, 255, 0));  // Set the current LED to white
      ws2812b.show();                                                 // Update the strip
      currentLED = (currentLED + 1) % NUM_PIXELS;                     // Move to the next LED
    }
    if (client.connect(host, port)) {
      setColor(2);
      ws2812b.show();
    }
  }
  if (client.available()) {
    String response = client.readStringUntil('\n');
    if (response.length() > 0) {
      Serial.println(response);
      int startIndex = response.indexOf(':') + 2;
      int endIndex = response.indexOf('%');
      if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
        String percentString = response.substring(startIndex, endIndex);
        volumePercent = percentString.toFloat();
        Serial.print("Volume: ");
        Serial.println(volumePercent);
        if (volumePercent > 0) {
          ws2812b.setBrightness(volumePercent);
          ws2812b.show();
        }
      }
      if (response.startsWith("set_control_values")) {
        sscanf(response.c_str(), "set_control_values %d %f,%f,%f %f %f", &nrOfAttractors, &PID_P, &PID_I, &PID_D, &PID_ramp, &motorVoltageLimit);
        P_haptic.P = PID_P;
        motor.voltage_limit = motorVoltageLimit;
        Serial.println("Control values updated");
      } else if (response == "get_control_values") {
        String callBack = "control_values:" + String(nrOfAttractors) + "," + String(PID_P) + "," + String(PID_I) + "," + String(PID_D) + "," + String(PID_ramp) + "," + String(motorVoltageLimit);
        client.println(callBack);
        Serial.println("Sent control values");
      }
    }
  }
}
