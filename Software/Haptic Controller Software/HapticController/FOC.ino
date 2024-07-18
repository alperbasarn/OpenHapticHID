MagneticSensorI2C sensor = MagneticSensorI2C(MT6701_I2C);
//MagneticSensorI2C sensor = MagneticSensorI2C(AS5048_I2C);

BLDCDriver3PWM driver = BLDCDriver3PWM(6, 7, 8, 5);
// haptic attraction controller - only Proportional
const float alpha = 0.6;    // Low-pass filter coefficient
int currentColorIndex = 0;  // Variable to keep track of the current color index
int clickStartTime = 0;
int clickDuration = 10;                 //ms
int attractAngleLockDuration = 50;      //ms
unsigned long lastDirChangeTime = 500;  // Last time attract_angle changed
unsigned long attractAngleLastChangeTime = 0;
unsigned long attractAngleChangeInterval = 10;

const int changeInterval = 10;

float attract_angle = 0.00;
float filtered_angle = 0.00;
float _filtered_angle = 0.00;
float _attract_angle = 0.00;
float measured_attract_angle = 0.00;
float _measured_attract_angle = 0.00;
// distance between attraction points
float attractor_distance = 360 / nrOfAttractors;  // dimp each 45 degrees
float targetAngle = 0.0;
float rawangle = 0.0;
float sensorReadout = 0.0;
float error = 0;
float _error = 0;
float deadZone = attractor_distance / 3;

const int consistencyThreshold = 50;  // Threshold in milliseconds
unsigned long lastSettleTime = 0;
bool angleSettled = false;
const float tolerance = 1.0;  // Tolerance range for angle settling
bool motorClicked = false;
bool angleConsistent = false;


void initFOC() {

  sensor.init();
  motor.linkSensor(&sensor);
  driver.pwm_frequency = 50000;
  driver.voltage_power_supply = 5;
  driver.init();
  motor.linkDriver(&driver);
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor.controller = MotionControlType::torque;
  motor.velocity_limit = 2;
  motor.voltage_limit = motorVoltageLimit;
  motor.voltage_sensor_align = 3;
  // init motor hardware
  motor.init();
  motor.initFOC();
  motor.useMonitoring(Serial);
}

void updateFOC() {
  static float filtered_angle = 0.0;
  const float alpha = 0.1;  // Low-pass filter coefficient

  //sensor.update();
  //rawangle = sensor.getSensorAngle();
  rawangle = sensor.getRawCount() / 16384.00 * 360.00;
  filtered_angle = rawangle;
  //lowPassFilter(rawangle, filtered_angle, alpha);  // Apply low-pass filter
  //Serial.println(filtered_angle);

  measured_attract_angle = findAttractor(filtered_angle);
  error = P_haptic(attract_angle - filtered_angle);


  // Check if the filtered angle is within the tolerance range of the attract angle
  if (abs(filtered_angle - measured_attract_angle) <= tolerance) {
    if (!angleConsistent) {
      lastSettleTime = millis();
      angleConsistent = true;
    } else if (millis() - lastSettleTime > consistencyThreshold && !motorClicked) {
      attract_angle = measured_attract_angle;
      clickMotor();  // Click the motor after settling on the new attract angle
      _attract_angle = attract_angle;
      motorClicked = true;  // Ensure the motor clicks only once per settle
    }
  } else {
    angleConsistent = false;
    motorClicked = false;
  }

  // Resetting and managing other states if needed
  if (attract_angle < _attract_angle) {
    Serial.println("kcw");
  }
  if (attract_angle > _attract_angle) {
     Serial.println("kccw");
  }

  _measured_attract_angle = measured_attract_angle;  // Update the stored attract angle
}

float findAttractor(float current_angle) {
  return round((current_angle / attractor_distance)) * attractor_distance;
}

float lowPassFilter(float newValue, float oldValue, float alpha) {
  return alpha * newValue + (1 - alpha) * oldValue;
}

void clickMotor() {
  // Implement the motor "click" action here
  // For example, move the motor a small amount and then back to the original position
  float clickDistance = 1;  // Define the click distance, adjust as necessary
  unsigned long clickStartTime = millis();

  // Move the motor to the attract angle and hold for a short duration
  while (millis() - clickStartTime < 300) {  // Use a short duration for the click action
    motor.loopFOC();
    motor.move(attract_angle);
    Serial.println("click");
  }
}
