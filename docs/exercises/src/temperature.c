#include "temperature.h"
#include "i2c.h"

/*
 * Provided and correct. In Exercise 3 you write tests for this function while
 * mocking i2c_read_raw(), so no real sensor is needed.
 */
int temperature_read_celsius(void)
{
    uint8_t raw = i2c_read_raw(TEMPERATURE_REG);
    return (int)raw - 40;
}
