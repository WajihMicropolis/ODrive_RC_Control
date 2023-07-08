#include "RC_Control.hpp"

ServoInputPin<SteeringSignalPin> steering(PulseMin, PulseMax);
ServoInputPin<ThrottleSignalPin> throttle(PulseMin, PulseMax);
// ServoInputPin<Chan3SignalPin> Chan3(PulseMin, PulseMax);
// ServoInputPin<Chan4SignalPin> Chan4(PulseMin, PulseMax);

void RC_Init()
{
  while (!ServoInput.available())
  { // wait for all signals to be ready
    Serial.println("Waiting for servo signals...");
    delay(500);
  }
}

RCRX getRC(bool debug)
{
  RCRX RCValues;
  RCValues.Throttle = throttle.map(-100, 100);
  RCValues.Steering = steering.map(-100, 100);
  // RCValues.Chan3 = Chan3.getBoolean();
  // RCValues.Chan4 = Chan3.getBoolean();

  if (RCValues.Throttle < 10 && RCValues.Throttle > -10)
    RCValues.Throttle = 0;

  if (RCValues.Steering < 10 && RCValues.Steering > -10)
    RCValues.Steering = 0;

  if (!debug)
  {
    return RCValues;
  }
  Serial.printf("Steering: %d %% \n", RCValues.Steering);
  Serial.printf("Throttle: %d %% \n", RCValues.Throttle);
  //   Serial.printf("Chan3: %d \n", RCValues.Chan3);
  // Serial.printf("Chan4: %d \n", RCValues.Chan4);
  Serial.println("-----------------");
  return RCValues;
}
