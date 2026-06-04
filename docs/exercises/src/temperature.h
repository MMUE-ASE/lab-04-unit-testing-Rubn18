#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

/*
 * temperature.h — the module under test in Exercise 3.
 *
 * temperature_read_celsius() reads a raw byte from the sensor over I2C and
 * converts it to degrees Celsius. It DEPENDS on i2c_read_raw(), which you will
 * replace with a CMock mock so the test controls what the "sensor" returns.
 */

#define TEMPERATURE_REG 0x07U

/* Convert the sensor reading to Celsius. The sensor returns degrees with a
 * fixed -40 offset, i.e. celsius = raw - 40. */
int temperature_read_celsius(void);

#endif /* TEMPERATURE_H */
