#include "ODrive.hpp"

ODriveArduino odrive(Serial1);

float vel_limit = 1.0f,
      current_lim = 11.0f;

float velocity0 = 0,
      velocity1 = 0;

float Global_Right_Motor_Position,
    Global_Left_Motor_Position,
    Global_Right_Motor_Position_Prev,
    Global_Left_Motor_Position_Prev,
    Global_Right_Motor_Speed,
    Global_Left_Motor_Speed;

unsigned long getVelMillis = 0;

void ODrive_Init()
{
    Serial1.begin(BAUDRATE, SERIAL_8N1, ESP32_UART2_PIN_TX, ESP32_UART2_PIN_RX);

    Serial.println("Ready!");
}

void ODriveRCControl(int RC_Throttle, int RC_Steering)
{
    float throttle = 0,
          steering = 0;

    if (RC_Throttle > 0)
    {
        throttle = map(RC_Throttle, minThrPer, maxThrPer, minMotorSpeed, maxMotorFwSpeed);
    }
    else if (RC_Throttle < 0)
    {
        throttle = map(abs(RC_Throttle), minThrPer, maxThrPer, minMotorSpeed, maxMotorBwSpeed);
        throttle = -1 * throttle;
    }
    else
        throttle = 0;

    if (RC_Steering > 0)
    {
        steering = map(RC_Steering, minSteerPer, maxSteerPer, minMotorSpeed, maxSteerspeed);
    }
    else if (RC_Steering < 0)
    {
        steering = map(abs(RC_Steering), minSteerPer, maxSteerPer, minMotorSpeed, maxSteerspeed);
        steering = -1 * steering;
    }
    else
        steering = 0;
        
    ODriveSetSpeed(throttle, steering);
}

void ODriveSetSpeed(float Linear_x, float Angle_z)
{
    float Center_Of_Rotation,
        Left_Wheel_Velocity,
        Right_Wheel_Velocity,
        Left_Wheel_Velocity_In_RPS,
        Right_Wheel_Velocity_In_RPS;

    if (Angle_z != 0.0)
    {
        Center_Of_Rotation = Linear_x / Angle_z;

        // Formula for converting the velocity cmd twist msg to motor velocity
        Left_Wheel_Velocity = -1 * (Angle_z * (Center_Of_Rotation - WheelBase / 2));
        Right_Wheel_Velocity = Angle_z * (Center_Of_Rotation + WheelBase / 2);

        Left_Wheel_Velocity_In_RPS = Left_Wheel_Velocity / (2 * 3.14 * WheelRadius);
        Right_Wheel_Velocity_In_RPS = Right_Wheel_Velocity / (2 * 3.14 * WheelRadius);
    }
    else
    {
        Left_Wheel_Velocity_In_RPS = Right_Wheel_Velocity_In_RPS = Linear_x / (2 * 3.14 * WheelRadius);
        Left_Wheel_Velocity_In_RPS = -1 * (Left_Wheel_Velocity_In_RPS);
    }

    odrive.SetVelocity(RightMotor, Right_Wheel_Velocity_In_RPS);
    odrive.SetVelocity(LeftMotor, Left_Wheel_Velocity_In_RPS);

    Serial.printf("RightMotor Speed: %f  \n", Right_Wheel_Velocity_In_RPS);
    Serial.printf("LeftMotor Speed: %f  \n", Left_Wheel_Velocity_In_RPS);
    Serial.println("-----------------");
}

void ODriveGetVelocity()
{
    if (millis() - getVelMillis > 16)
    {

        Global_Right_Motor_Position_Prev = Global_Right_Motor_Position;
        Global_Left_Motor_Position_Prev = Global_Left_Motor_Position;

        Global_Right_Motor_Speed = odrive.GetVelocity(RightMotor);
        Global_Left_Motor_Speed = -1 * odrive.GetVelocity(LeftMotor);

        Global_Right_Motor_Position = odrive.GetPosition(RightMotor);
        Global_Left_Motor_Position = -1 * odrive.GetPosition(LeftMotor);

        getVelMillis = millis();
    }
}

void ODriveSpeedControl()
{
    // if (steering == 0.0)
    // {
    //     float vel0 = speed;
    //     float vel1 = -speed;
    //     Serial.printf("Velocity0 is set to: %f  \n", vel0);
    //     Serial.printf("Velocity1 is set to: %f  \n", vel1);

    //     odrive.SetVelocity(0, vel0);
    //     odrive.SetVelocity(1, vel1);
    // }
    // else
    // {
    //     float vel0 = steering;
    //     float vel1 = steering;
    //     Serial.printf("Velocity0 is set to: %f  \n", vel0);
    //     Serial.printf("Velocity1 is set to: %f  \n", vel1);

    //     odrive.SetVelocity(0, vel0);
    //     odrive.SetVelocity(1, vel1);
    // }
}
void ODriveSerialControl()
{
    if (Serial.available())
    {
        char c = Serial.read();
        Serial.println((String) " Serial.Read(): " + c);

        // Run calibration sequence
        if (c == '0' || c == '1')
        {
            int requested_state = AxisState::AXIS_STATE_CLOSED_LOOP_CONTROL;

            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            odrive.run_state(0, requested_state, false); // don't wait

            Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
            odrive.run_state(1, requested_state, false); // don't wait
        }

        else if (c == 'f')
        {
            velocity0 += 0.1;
            velocity1 -= 0.1;
            Serial.printf("Velocity is set to: %f", velocity0);

            odrive.SetVelocity(0, velocity0);
            odrive.SetVelocity(1, velocity1);
        }

        else if (c == 'r')
        {
            velocity0 -= 0.1;
            velocity1 += 0.1;
            Serial.printf("Velocity is set to: %f", velocity0);

            odrive.SetVelocity(0, velocity0);
            odrive.SetVelocity(1, velocity1);
        }

        // Read bus voltage
        if (c == 'b')
        {
            // odrive_serial << "r vbus_voltage\n";
            Serial1 << "r vbus_voltage\n";
            Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
        }
    }
}