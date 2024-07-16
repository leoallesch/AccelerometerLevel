#include "I2C.h"

void I2C_Init(uint32_t scl_freq) {
    // Set SCL frequency
    uint8_t prescaler = 0;
    TWCR0 = 0; // Reset control register
    TWSR0 = 0; // Reset status register
    TWBR0 = ((F_CPU / scl_freq) - 16) / (2 * (1 << prescaler)); // Bit rate

    // Enable TWI
    TWCR0 = (1 << TWEN);
}

void I2C_Start(void) {
    TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR0 & (1 << TWINT)));
}

void I2C_Stop(void) {
    TWCR0 = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    while (TWCR0 & (1 << TWSTO));
}

void I2C_Write(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR0 & (1 << TWINT)));
}

uint8_t I2C_ReadAck(void) {
    TWCR0 = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR0 & (1 << TWINT)));
    return TWDR0;
}

uint8_t I2C_ReadNack(void) {
    TWCR0 = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR0 & (1 << TWINT)));
    return TWDR0;
}

uint8_t I2C_GetStatus(void) {
    uint8_t status;
    status = TWSR0 & 0xF8;
    return status;
}
