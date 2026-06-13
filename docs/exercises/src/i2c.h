#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * i2c.h — a hardware dependency for Exercise 3.
 *
 * There is NO i2c.c in this project. On the real board this would talk to an
 * I2C peripheral over real wires — something a host PC cannot do. That is
 * exactly the situation CMock is built for: it generates a fake ("mock")
 * implementation of these functions from this header, so you can test code
 * that DEPENDS on I2C without any hardware.
 *
 * In your test you include "mock_i2c.h" instead of "i2c.h".
 */

/* Read one raw byte from a register of the temperature sensor. */
uint8_t i2c_read_raw(uint8_t reg);

#endif /* I2C_H */
