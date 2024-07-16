#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

// Function prototypes
void I2C_Init(uint32_t scl_freq);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNack(void);
uint8_t I2C_GetStatus(void);

#endif
