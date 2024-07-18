void drawStaticVolumeElements() {

  // Draw the title
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->setCursor(20, 140);
  int16_t x1, y1;
  uint16_t w, h;
  String demoString = "SOUND";
  gfx->getTextBounds(demoString, 0, 0, &x1, &y1, &w, &h);
  gfx->setCursor((240 - w) / 2, (150 - h) / 2);
  gfx->print(demoString);
  gfx->setTextSize(2);
  // Draw arrows
  drawUpDownArrow(70, 110, false);  // Left arrow for decreasing
  drawUpDownArrow(170, 110, true);  // Right arrow for increasing
  drawBackButton();
  // Draw the labels for the arrows
  //gfx->setTextSize(2);
  //gfx->setCursor(10, 115);
  //gfx->print("DOWN");
  //gfx->setCursor(200, 115);
  //gfx->print("UP");
}
void drawSetPoint() {
  // Clear the previous setpoint value
  gfx->fillRect(75, 105, 90, 30, BLACK);

  // Draw the setpoint value in the middle
  gfx->setTextSize(4);
  gfx->setTextColor(WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  String setpointStr =   String(setpoint)+"%";
  gfx->getTextBounds(setpointStr, 0, 0, &x1, &y1, &w, &h);
  gfx->setCursor((240 - w) / 2, (240 - h) / 2);
  gfx->print(setpointStr);
  drawVolumeBars(setpoint);
  lastSetpoint = setpoint;
}



void drawUpDownArrow(int x, int y, bool up) {
  if (up) {
    gfx->fillTriangle(x - 10, y + 10, x + 10, y + 10, x, y - 20, WHITE);  // Up arrow
  } else {
    gfx->fillTriangle(x - 10, y - 10, x + 10, y - 10, x, y + 20, WHITE);  // Down arrow
  }
}
uint16_t adjustBrightness(uint16_t color, float brightness) {
  uint8_t r = ((color >> 11) & 0x1F) * brightness;
  uint8_t g = ((color >> 5) & 0x3F) * brightness;
  uint8_t b = (color & 0x1F) * brightness;
  return gfx->color565(r << 3, g << 2, b << 3);
}

uint16_t interpolateColor(uint16_t color1, uint16_t color2, float fraction) {
  uint8_t r1 = (color1 >> 11) & 0x1F;
  uint8_t g1 = (color1 >> 5) & 0x3F;
  uint8_t b1 = color1 & 0x1F;
  
  uint8_t r2 = (color2 >> 11) & 0x1F;
  uint8_t g2 = (color2 >> 5) & 0x3F;
  uint8_t b2 = color2 & 0x1F;
  
  uint8_t r = r1 + fraction * (r2 - r1);
  uint8_t g = g1 + fraction * (g2 - g1);
  uint8_t b = b1 + fraction * (b2 - b1);
  
  return gfx->color565(r << 3, g << 2, b << 3);
}

void drawVolumeBars(int volume) {
  int centerX = 120;
  int centerY = 120;
  int outerRadius = 140;
  int innerRadius = 70;
  float gap = 6.0; // Increased gap between bars

  // Calculate the angle step between each bar
  float angleStep = (300.0 / barCount) - gap;  // Cover 300 degrees with gaps

  int fullBars = volume / 5;
  float partialBarBrightness = (volume % 5) / 5.0;

  uint16_t startColor = gfx->color565(255, 0, 0); // Red
  uint16_t endColor = gfx->color565(0, 255, 0);   // Green

  for (int i = 0; i < barCount; i++) {
    // Calculate the angles for the current bar
    float angle1 = (120 + i * (angleStep + gap)) * PI / 180.0;
    float angle2 = (120 + (i + 1) * angleStep + i * gap) * PI / 180.0;

    // Calculate the points of the polygon (quadrilateral)
    int x1 = centerX + innerRadius * cos(angle1);
    int y1 = centerY + innerRadius * sin(angle1);
    int x2 = centerX + outerRadius * cos(angle1);
    int y2 = centerY + outerRadius * sin(angle1);
    int x3 = centerX + outerRadius * cos(angle2);
    int y3 = centerY + outerRadius * sin(angle2);
    int x4 = centerX + innerRadius * cos(angle2);
    int y4 = centerY + innerRadius * sin(angle2);

    // Interpolate color based on the bar index
    float fraction = (float)i / (barCount - 1);
    uint16_t color = interpolateColor(startColor, endColor, fraction);

    // Adjust brightness if this is the partial bar
    if (i == fullBars) {
      color = adjustBrightness(color, partialBarBrightness);
    } else if (i > fullBars) {
      color = BLACK; // Set inactive bars to black
    }

    // Draw the polygon
    gfx->fillTriangle(x1, y1, x2, y2, x3, y3, color);
    gfx->fillTriangle(x1, y1, x3, y3, x4, y4, color);
  }
}