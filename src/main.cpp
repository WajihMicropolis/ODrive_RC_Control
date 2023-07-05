#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
ServoInputPin<SteeringSignalPin> steering(PulseMin, PulseMax);
ServoInputPin<ThrottleSignalPin> throttle(PulseMin, PulseMax);

void setup()
{
  Serial.begin(115200);

  while (!ServoInput.available())
  { // wait for all signals to be ready
    Serial.println("Waiting for servo signals...");
    delay(500);
  }
  ODrive_Init();
}

RCRX getRC(bool debug = false)
{
  RCRX RCValues;
  RCValues.Throttle = throttle.map(-100, 100);
  RCValues.Steering = steering.map(-100, 100);
  // RCValues.Chan3 = Chan3.getBoolean();
  // RCValues.Chan4 = Chan3.getBoolean();
  if (!debug)
  {
    return RCValues;
  }
  Serial.printf("Steering: %d% \t", RCValues.Steering);
  Serial.printf("Throttle: %d% \t", RCValues.Throttle);
  Serial.printf("Chan3: %d% \n", RCValues.Chan3);
  // Serial.printf("Chan4: %d% \n", RCValues.Chan4);
  return RCValues;
}

void loop()
{
  // ODriveSerialControl();

  RCRX RC;
  RC = getRC();
  float throttle = 0,
        steering = 0;
  if (RC.Throttle >= 10)
  {
    throttle = map(RC.Throttle, 10, 100, 0, 100);
    throttle = throttle / 100;
  }
  else if (RC.Throttle <= -10)
  {
    throttle = map(-RC.Throttle, 10, 100, 0, 100);
    throttle = -throttle / 100;
  }
  else
    throttle = 0;

  if (RC.Steering >= 10)
  {
    steering = map(RC.Steering, 10, 100, 0, 100);
    steering = steering / 100;
  }
  else if (RC.Steering <= -10)
  {
    steering = map(-RC.Steering, 10, 100, 0, 100);
    steering = -steering / 100;
  }
  else
    steering = 0;

  // Serial.println(steering);
  ODriveSpeedControl(throttle, steering);
}