#include <Adafruit_NeoPixel.h>
#include <SimpleFOC.h>
#include <WiFi.h>
#define PIN_WS2812B 9  // ESP32 pin that connects to WS2812B
#define NUM_PIXELS 16  // The number of LEDs (pixels) on WS2812B

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
WiFiClient client;
BLDCMotor motor = BLDCMotor(8);

float nrOfAttractors = 10;

float PID_P = 0.3;
float PID_I = 0;
float PID_D = 0;
float PID_ramp = 10;
float PID_limit = 2;
float motorVoltageLimit = 4;
PIDController P_haptic{ .P = PID_P, .I = PID_I, .D = PID_D, .output_ramp = PID_ramp, .limit = PID_limit };

float volumePercent = 0;
int currentColor = 1;  // RGBW
int demoMode = 1;      //Windows volume control, 2 fusion 360 rotation
int currentLED = 0;
int lastUpdate = millis();
int lastUpdateClient = millis();
float ledVal = 1.0;
int proxRead = 0;
int rRead = 0, gRead = 0, bRead = 0;


void setup() {
  Serial.begin(115200);
  Wire.end();
  Wire.setPins(4, 3);
  Wire.begin();
  //initWiFi();
  initFOC();
  //initTCPClient();
  //initLED();
  Wire.setClock(400000);
  motor.enable();
  delay(3000);
  initSensors();
  Serial.println("init complete");
}

void loop() {
 // handleClient();
  readSensors();
  //Serial.println("loop");
  updateFOC();
  //updateLED();
}
