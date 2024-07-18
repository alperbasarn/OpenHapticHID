#include "Adafruit_VEML7700.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <DFRobot_PAJ7620U2.h>
#include <SparkFun_APDS9960.h>

unsigned long sensorsLastUpdate = 0;
bool initapds = false;
DFRobot_PAJ7620U2 paj;
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;

Adafruit_BMP280 bmp;  // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void initSensors() {
  if (apds.init()) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Adjust the Proximity sensor gain
  if (!apds.setProximityGain(PGAIN_2X)) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }

  // Start running the APDS-9960 proximity sensor (no interrupts)
  if (apds.enableProximitySensor(true)) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }
  if (apds.enableGestureSensor(true)) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  if (!aht.begin()) {
    Serial.println("Failed to find AHT10/AHT20 chip");

  } else {
    Serial.println("AHT10/AHT20 Found!");
    aht_temp = aht.getTemperatureSensor();
    aht_temp->printSensorDetails();

    aht_humidity = aht.getHumiditySensor();
    aht_humidity->printSensorDetails();
  }


  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
  } else {
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    bmp_temp->printSensorDetails();
  }



  /*
  if (!veml.begin()) {
    Serial.println("VEML7700 not found");
  }
  if (!aht.begin()) {
    Serial.println("Failed to find AHT10/AHT20 chip");

  } else {
    Serial.println("AHT10/AHT20 Found!");
    aht_temp = aht.getTemperatureSensor();
    aht_temp->printSensorDetails();

    aht_humidity = aht.getHumiditySensor();
    aht_humidity->printSensorDetails();
  }
  Serial.println("Gesture recognition system based on PAJ7620U2");
  if (paj.begin() != 0) {
    Serial.println("initial PAJ7620U2 failure! Please check if all the connections are fine, or if the wire sequence is correct?");
  }
  Serial.println("PAJ7620U2 init completed, start to test the gesture recognition function");

  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }
  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
    */
}

void readSensors() {

  /*
  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold");
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold");
  }

  sensors_event_t humidity;
  sensors_event_t temp;
  aht_humidity->getEvent(&humidity);
  aht_temp->getEvent(&temp);
  DFRobot_PAJ7620U2::eGesture_t gesture = paj.getGesture();
  if (gesture != paj.eGestureNone) {
    String description = paj.gestureDescription(gesture);
    if (description == "Up") {
      nextColor();
    }
    //client.print(description);
    Serial.println(description);
    Serial.println();
    delay(100);
    }
    

  sensors_event_t humidity;
  sensors_event_t temp;
  sensors_event_t temp_event, pressure_event;
  aht_humidity->getEvent(&humidity);
  aht_temp->getEvent(&temp);
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");
  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");
*/
  if (!apds.readProximity(proximity_data)) {
    Serial.println("Error reading proximity value");
  } else {
    Serial.print("Proximity: ");
    Serial.println(proximity_data);
  }

  /*
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
    }
  }
  */
  sensorsLastUpdate = millis();
}