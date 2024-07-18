#include <Arduino_GFX_Library.h>
#define GFX_BL DF_GFX_BL  // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX* gfx = create_default_Arduino_GFX();
#else  /* !defined(DISPLAY_DEV_KIT) */
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus* bus = create_default_Arduino_DataBus();
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX* gfx = new Arduino_GC9A01(bus, DF_GFX_RST, 0 /* rotation /, false / IPS */);
#endif /* !defined(DISPLAY_DEV_KIT) */
       /*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

const int maxVolume = 100;  // Maximum volume level
const int minVolume = 0;   // Minimum volume level
const int barCount = 20;     // Number of bars


void drawBackButton() {
  int centerX = 120;
  int centerY = 210;
  int width = 40;
  int height = 20;
  gfx->fillEllipse(centerX, centerY, width, height, gfx->color565(128, 128, 128));  // Gray back button

  // Draw the arrow inside the back button
  gfx->fillTriangle(centerX - 5, centerY, centerX - 5, centerY - 10, centerX + 5, centerY + 10, WHITE);  // Black arrow
}

void initScreen() {

  //gfx->fillScreen(BLACK);
  //gfx->fillScreen(BLACK);
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif


#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  // Init Display
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }
  //drawTestCircle();
  //testColors();
  gfx->invertDisplay(true);
  gfx->fillScreen(BLACK);
}

void turnDisplayOff() {
gfx->displayOff();
digitalWrite(2, LOW);
displayIsOn = false;
}

void turnDisplayOn() {
gfx->displayOn();
digitalWrite(2, HIGH);
displayIsOn = true;
}









