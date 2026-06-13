#include "unity.h"
#include "temperature.h"
#include "mock_i2c.h"   /* CMock generates this from src/i2c.h */

/*
 * Exercise 3 — Mocking a dependency with CMock.
 *
 * temperature_read_celsius() calls i2c_read_raw(). There is no real sensor and
 * no i2c.c, so instead we include "mock_i2c.h": CMock auto-generated a fake
 * i2c_read_raw() that WE control.
 *
 * For every function in i2c.h, CMock gives you helpers. The two you need here:
 *
 *   i2c_read_raw_ExpectAndReturn(expected_reg, value_to_return);
 *       "I expect i2c_read_raw to be called once with expected_reg, and when it
 *        is, make it return value_to_return."
 *
 * After the test body, Ceedling automatically verifies every expectation was
 * met (called the right number of times, with the right arguments). If the code
 * never calls i2c_read_raw, or calls it with the wrong register, the test fails.
 *
 * Run it:  ceedling test:test_temperature
 */

void setUp(void) {}
void tearDown(void) {}

/* ----- Worked example --------------------------------------------------- */
void test_read_celsius_converts_raw_with_offset(void)
{
    /* Arrange: when the sensor is read at TEMPERATURE_REG, pretend it returns 65 */
    i2c_read_raw_ExpectAndReturn(TEMPERATURE_REG, 65);

    /* Act */
    int celsius = temperature_read_celsius();

    /* Assert: 65 - 40 == 25 */
    TEST_ASSERT_EQUAL_INT(25, celsius);
}

/* ----- TODO EX3.1 ------------------------------------------------------- *
 * Arrange the mock so the sensor returns 40, then assert the result is 0.
 */
void test_read_celsius_at_zero_degrees(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}

/* ----- TODO EX3.2 ------------------------------------------------------- *
 * A reading of 10 means -30 C. Arrange the mock to return 10 and assert -30.
 * Note how you never touch real hardware — the mock fully stands in for it.
 */
void test_read_celsius_below_zero(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}
