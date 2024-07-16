#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <util/delay.h>

#include "I2C.h"
#include "UART.h"
#include "mpu6050/MPU6050.h"

double AccelerationToDegrees(int16_t ay) {
    // Convert raw accelerometer value to 'g' units
    double ay_g = (double)ay / 16384.0;

    // Ensure the value is within the valid range for asin
    if (ay_g > 1.0) ay_g = 1.0;
    else if (ay_g < -1.0) ay_g = -1.0;

    // Convert y-axis acceleration to angle in radians
    double angle_rad = asin(ay_g);

    // Convert angle from radians to degrees
    double angle_deg = angle_rad * (180.0 / 3.142);

    return angle_deg;
}

void format_float(char *buffer, size_t size, double value) {
    int int_part = (int)value;
    int frac_part = (int)((value - int_part) * 100); // Two decimal places

    if (frac_part < 0) frac_part = -frac_part; // Handle negative fractional part

    snprintf(buffer, size, "%d.%02d", int_part, frac_part);
}


int main(void) {
    int16_t ax, ay, az;
    char buffer[64];
    char angle_str[20];
    int16_t zero = 0;

    I2C_Init(100000);
    UART_Init(9600);
    MPU6050_Init();

    UART_Transmit_String("MPU6050 Initialized\r\n");

    while (1) {
        MPU6050_ReadAccel(&ax, &ay, &az);

        if(!zero)
        {
            zero = ay;
        }

        double angle_deg = AccelerationToDegrees(ay-zero);

        // Format the angle as a string
        format_float(angle_str, sizeof(angle_str), angle_deg);

        // Format the output string
        snprintf(buffer, sizeof(buffer), "\33[2K\rAngle: %s degrees", angle_str);

        // Transmit the formatted string
        UART_Transmit_String(buffer);

        _delay_ms(250);
    }

    return 0;
}