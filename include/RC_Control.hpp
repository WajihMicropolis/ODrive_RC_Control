#ifndef _RC_CONTROL_HPP
#define _RC_CONTROL_HPP
#include <Arduino.h>
#include <ServoInput.h>

const int PulseMin = 1000,
          PulseMax = 2000;

// MUST be interrupt-capable!
const int SteeringSignalPin = 5,
          ThrottleSignalPin = 18,
          Chan3SignalPin = 17, // chan3 Setup
          Chan4SignalPin = 18;       // chan4 Setup

struct RCRX
{
  int Steering;
  int Throttle;
  bool Chan3;
  bool Chan4;
};
void RC_Init();
RCRX getRC(bool debug = false);

#endif //_RC_CONTROL_HPP