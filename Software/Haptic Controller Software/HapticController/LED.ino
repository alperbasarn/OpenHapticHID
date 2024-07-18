

// Helper function to set the color of all pixels
void writeColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < ws2812b.numPixels(); i++) {
    ws2812b.setPixelColor(i, ws2812b.Color(red, green, blue));
  }
  ws2812b.show();  // Send the updated color to the hardware
}

// Function to set the color
void setColor(int i) {
  currentColor = i;
  switch (i) {
    case 1:
      writeColor(255, 0, 0);  // Set all pixels to red
      break;
    case 2:
      writeColor(0, 255, 0);  // Set all pixels to green
      break;
    case 3:
      writeColor(0, 0, 255);  // Set all pixels to blue
      break;
    case 4:
      writeColor(255, 255, 255);  // Set all pixels to white
    case 5:
      writeColor(0, 255, 255);  // Set all pixels to yellow
      break;
  }
}

// Function to cycle to the next color
void nextColor() {
  currentColor = (currentColor + 1) % 5;  // Move to the next color, wrap around after white
  setColor(currentColor);                 // Set all pixels to the new current color
}

void initLED() {

  ws2812b.begin();  // Initialize WS2812B strip
  ws2812b.clear();
  ws2812b.setBrightness(ledVal);  // Set initial brightness
  ws2812b.setBrightness(100);     // Set maximum brightness for full color range

  // Precompute the colors for each LED
  uint8_t red[NUM_PIXELS], green[NUM_PIXELS], blue[NUM_PIXELS];
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    float position = (float)pixel / (NUM_PIXELS - 1);  // Position in the strip

    if (position < 0.2) {
      // Red to Yellow
      red[pixel] = 255;
      green[pixel] = map(position * 10, 0, 2, 0, 255);
      blue[pixel] = 0;
    } else if (position < 0.4) {
      // Yellow to Green
      red[pixel] = map(position * 10, 2, 4, 255, 0);
      green[pixel] = 255;
      blue[pixel] = 0;
    } else if (position < 0.6) {
      // Green to Cyan
      red[pixel] = 0;
      green[pixel] = 255;
      blue[pixel] = map(position * 10, 4, 6, 0, 255);
    } else if (position < 0.8) {
      // Cyan to Blue
      red[pixel] = 0;
      green[pixel] = map(position * 10, 6, 8, 255, 0);
      blue[pixel] = 255;
    } else {
      // Blue to Violet
      red[pixel] = map(position * 10, 8, 10, 0, 255);
      green[pixel] = 0;
      blue[pixel] = 255;
    }
  }

  // Animation loop for simultaneous glowing
  for (int t = 0; t < 5000; t += 20) {  // Run for 5000 ms (5 seconds) in steps of 20 ms
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      // Frequency range from 5 Hz to 15 Hz
      float frequency = 5.0 + (10.0 * (float)pixel / (NUM_PIXELS - 1));
      float brightnessFactor = 170 + (sin(t * frequency * 2 * PI / 1000) * 30 + 30);  // Fluctuate between 10 and 70

      // Set the color with the fluctuated brightness
      ws2812b.setPixelColor(pixel, ws2812b.Color(
                                     (red[pixel] * brightnessFactor) / 255,
                                     (green[pixel] * brightnessFactor) / 255,
                                     (blue[pixel] * brightnessFactor) / 255));
    }
    ws2812b.show();
    delay(20);  // 20 ms delay for smooth animation
  }
  ws2812b.setBrightness(ledVal);
  ws2812b.show();
}

void updateLED() {
  ws2812b.setBrightness(ledVal);
  ws2812b.show();
}