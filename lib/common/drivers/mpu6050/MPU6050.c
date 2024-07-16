#include "MPU6050.h"
#include "328pb/I2C.h"

void MPU6050_Init(void) {
    I2C_Start();
    I2C_Write(MPU6050_ADDRESS << 1); // Write address
    I2C_Write(MPU6050_PWR_MGMT_1);   // Power management register
    I2C_Write(0x00);                 // Wake up MPU6050
    I2C_Stop();
}

void MPU6050_ReadAccel(int16_t* ax, int16_t* ay, int16_t* az) {
    uint8_t data[6];

    I2C_Start();
    I2C_Write(MPU6050_ADDRESS << 1); // Write address
    I2C_Write(MPU6050_ACCEL_XOUT_H); // Starting register to read
    I2C_Start();
    I2C_Write((MPU6050_ADDRESS << 1) | 1); // Read address

    for (uint8_t i = 0; i < 5; i++) {
        data[i] = I2C_ReadAck();
    }
    data[5] = I2C_ReadNack();
    I2C_Stop();

    *ax = ((int16_t)data[0] << 8) | data[1];
    *ay = ((int16_t)data[2] << 8) | data[3];
    *az = ((int16_t)data[4] << 8) | data[5];
}
