
#include <ODriveArduino.h>

// ## I have not tried it yet, but if you need another serial: UART1, redefine pins (see video link above)
#define ESP32_UART2_PIN_TX 17
#define ESP32_UART2_PIN_RX 16

// ODrive uses 115200 baud
#define BAUDRATE 115200
float velocity0 = 0,
      velocity1 = 0;
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

// ODrive object //HardwareSerial Serial1;
ODriveArduino odrive(Serial1);

float vel_limit = 1.0f;
float current_lim = 11.0f;

void ODrive_Init()
{
    // Serial to the ODrive //UART HWSerial 1 Setup
    // Note: you must also connect GND on ODrive to GND on ESP32!
    Serial1.begin(BAUDRATE, SERIAL_8N1, ESP32_UART2_PIN_TX, ESP32_UART2_PIN_RX);
    // ## You should be able to setup another serial for more motors

    //  ODriveArduino odrive(Serial1);
    // Set current and velocity defaults
    for (int axis = 0; axis < 2; ++axis)
    {
        Serial1 << "w axis" << axis << ".controller.config.vel_limit " << vel_limit << '\n';
        Serial1 << "w axis" << axis << ".motor.config.current_lim " << current_lim << '\n';

        Serial << "Axis" << axis << ": Requesting state " << AxisState::AXIS_STATE_CLOSED_LOOP_CONTROL << '\n';
        odrive.run_state(axis, AxisState::AXIS_STATE_CLOSED_LOOP_CONTROL, false); // don't wait
    }

    // Some serial out documentation
    Serial.println("Ready!");
    Serial.println("Send the character '0' or '1' to calibrate respective motor (you must do this before you can command movement)");
    Serial.println("Send the character 's' to exectue test move");
    Serial.println("Send the character 'b' to read bus voltage");
    Serial.println("Send the character 'p' to read motor positions in a 10s loop");
    Serial.println("Send the character 'c'(-) or 'C'(+) to raise and lower the current_limit (+- 5)");
    Serial.println("Send the character 'v'(-) or 'V'(+) to raise and lower the velocity_limit (+- 50000)");
    Serial.println("Send the character 'x'(-) or 'X'(+) to switch back and forth to position (+- 5000)");
}

void ODriveSpeedControl(float speed, float steering)
{
    if (steering == 0.0)
    {
        float vel0 = speed;
        float vel1 = -speed;
        Serial.printf("Velocity0 is set to: %f  \n", vel0);
        Serial.printf("Velocity1 is set to: %f  \n", vel1);

        odrive.SetVelocity(0, vel0);
        odrive.SetVelocity(1, vel1);
    }
    else
    {
        float vel0 = steering;
        float vel1 = steering;
        Serial.printf("Velocity0 is set to: %f  \n", vel0);
        Serial.printf("Velocity1 is set to: %f  \n", vel1);

        odrive.SetVelocity(0, vel0);
        odrive.SetVelocity(1, vel1);
    }
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