#include <driver/rtc_io.h>
#include <esp_sleep.h>
#define WAKEUP_PIN GPIO_NUM_5

RTC_DATA_ATTR int bootCount = 0;
// Center points of the left and right circles
int leftCircleX = 50;
int leftCircleY = 120;

int rightCircleX = 190;
int rightCircleY = 120;
//touch screen params
bool released;
bool hold;
bool pressed;
bool volumeMode = true;
bool volumeModeInitialized = false;
int currentTime = 0;  //ms
int setpoint = 50;
int lastSetpoint = 0;
bool displayIsOn = true;
bool sleeping = false;
void setup(void) {
  Serial.begin(115200);
  Serial.println("Booting");
  //initWifi();
  initScreen();
  initTouchPanel();
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up due to external wakeup (pin 5).");
  } else {
    Serial.println("Not a deep sleep wakeup.");
  }
}

void loop() {
  handleOTA();
  handleTouchPanel();
  if (volumeMode) {
    if (!volumeModeInitialized) {
      drawStaticVolumeElements();
      volumeModeInitialized = true;
    }
    if (setpoint != lastSetpoint) {
      drawSetPoint();
    }
  }
  if (hold) {
    if (!sleeping) {
      delay(100);
      checkVolumeButtons();
      goToSleep();
    }
  }
  if (sleeping) {
    if (pressed) {
      pressed = false;
      delay(3000);
      handleTouchPanel();
      wakeUp();
      keepSleeping();
    }
  }
}

void goToSleep() {
  //turnDisplayOff();
  hold = false;
  pressed = false;
  sleeping = true;
  Serial.println("Going to sleep now...");
  // Configure the wakeup source as pin 5
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);  // Wake up on low level
  delay(1000);
  // Go to deep sleep
  esp_deep_sleep_start();
}

void wakeUp() {
  //turnDisplayOn();
  sleeping = false;
}
void keepSleeping() {
}