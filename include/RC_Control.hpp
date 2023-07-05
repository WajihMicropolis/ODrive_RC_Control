#ifndef _RC_CONTROL_HPP
#define _RC_CONTROL_HPP

#include <ServoInput.h>

const int PulseMin = 1000,
          PulseMax = 2000;

// MUST be interrupt-capable!
const int SteeringSignalPin = 5,
          ThrottleSignalPin = 18,
          Chan3SignalPin = 17, // chan3 Setup
          Chan4SignalPin = 18;       // chan4 Setup

// ServoInputPin<ThrottleSignalPin> throttle(PulseMin, PulseMax);
// ServoInputPin<SteeringSignalPin> steering(PulseMin, PulseMax);
// ServoInputPin<Chan3SignalPin> Chan3(PulseMin, PulseMax);
// ServoInputPin<Chan4SignalPin> Chan4(PulseMin, PulseMax);

struct RCRX
{
  int Steering;
  int Throttle;
  bool Chan3;
  bool Chan4;
};

#endif //_RC_CONTROL_HPP