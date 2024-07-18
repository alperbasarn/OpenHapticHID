#include <CST816S.h>

bool pressDetected;
bool _pressDetected;

bool displayOn;
int pressTime = 0;           // ms time when press detected
int releaseTime = 0;         // ms time when release detected
int pressDuration = 0;       //ms actual duration of press event
int holdThreshold = 1000;    //ms threshold duration to determine hold
int releaseThreshold = 100;  //ms threshold duration to determine hold
int touchX = 0;
int touchY = 0;

CST816S touch(6, 7, 19, 5);  // sda, scl, rst, irq

void initTouchPanel() {
  touch.begin();
}

void checkVolumeButtons() {

  if (abs(touchX - leftCircleX) < 30 && abs(touchY - leftCircleY) < 50) {
    setpoint--;
    Serial.println("Volume Decreased");
    delay(30);
  }

  // Check if the touch is within the right oval
  if (abs(touchX - rightCircleX) < 30 && abs(touchY - rightCircleY) < 50) {
    setpoint++;
    Serial.println("Volume Increased");
    delay(30);
  }
}
void handleTouchPanel() {
  currentTime = millis();
  if (touch.available()) {
    touchX = touch.data.x;
    touchY = touch.data.y;
    pressDetected = true;
    if (!pressed) {
      pressDetected = true;
      pressed = true;
      released = false;
      pressTime = millis();
      Serial.println("pressed");
      checkVolumeButtons();
    } else {
      if (!hold) {
        if (currentTime - holdThreshold > pressTime) {
          hold = true;
        }
      }
    }
  } else {
    pressDetected = false;
    if (_pressDetected) {
      releaseTime = millis();
    }
    if (!released) {
      if (currentTime - releaseThreshold > releaseTime) {
        hold = false;
        pressed = false;
        released = true;
        Serial.println("released");
      }
    }
  }

  _pressDetected = pressDetected;
}