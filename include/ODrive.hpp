#ifndef _ODRIVE_CONTROL_HPP_
#define _ODRIVE_CONTROL_HPP_

#include <ODriveArduino.h>
#include <Arduino.h>

#define ESP32_UART2_PIN_TX 17
#define ESP32_UART2_PIN_RX 16
#define BAUDRATE 115200

#define minThrPer 10
#define maxThrPer 100
#define minMotorSpeed 0
#define maxMotorFwSpeed 2
#define maxMotorBwSpeed 1

#define minSteerPer 10
#define maxSteerPer 100
#define maxSteerspeed 2

#define WheelRadius 0.158
#define WheelBase 0.3872

#define RightMotor 0
#define LeftMotor 1

// Printing with stream operator
template <class T>
inline Print &operator<<(Print &obj, T arg)
{
    obj.print(arg);
    return obj;
}
template <>
inline Print &operator<<(Print &obj, float arg)
{
    obj.print(arg, 4);
    return obj;
}

void ODrive_Init();
void ODriveSerialControl();

void ODriveRCControl(int RC_Throttle, int RC_Steering);
void ODriveSetSpeed(float Linear_x, float Angle_z);
void ODriveGetVelocity();



#endif //_ODRIVE_CONTROL_HPP_