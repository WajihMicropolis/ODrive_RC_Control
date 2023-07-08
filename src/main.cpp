#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"

void setup()
{
  Serial.begin(BAUDRATE);

  RC_Init();
  ODrive_Init();
}

void loop()
{

  RCRX RC;
  RC = getRC();

  ODriveRCControl(RC.Throttle, RC.Steering);
  ODriveGetVelocity();
}