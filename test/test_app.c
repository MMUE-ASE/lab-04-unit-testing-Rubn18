#include "unity.h"
#include "mock_gpio.h"   /* CMock generates this from your inc/gpio.h */
#include "app.h"
#include "board.h"

/*
 * Phase 3 — Test app.c by mocking the GPIO driver.
 *
 * app.c (src/app.c) is the code under test. It calls gpio_* functions. By
 * including "mock_gpio.h" we replace the real driver with mocks, so this test
 * verifies app.c's LOGIC — which pins it touches, in which order, with which
 * values — without any hardware and without depending on gpio.c being correct.
 *
 * The mock helpers you need:
 *   gpio_read_ExpectAndReturn(port, pin, value)
 *       expect one call to gpio_read(port, pin); make it return value.
 *   gpio_write_Expect(port, pin, value)
 *       expect one call to gpio_write(port, pin, value).
 *   gpio_enable_clock_Expect(port) / gpio_config_output_Expect(port, pin) / ...
 *
 * Calls must occur in the order you declare them (strict ordering is on).
 * After the test body, Ceedling checks every expectation was met exactly.
 *
 * Design note — why board.h constants instead of literals:
 *   The expectations use B1_PORT, B1_PIN, LD2_PORT, LD2_PIN from board.h.
 *   If you ever rewire a pin, changing board.h automatically fixes these
 *   tests too — no magic numbers to hunt down.
 *
 * Run it:  make test    (or: ceedling test:test_app)
 */

void setUp(void) {}
void tearDown(void) {}

/* ----- Worked example --------------------------------------------------- *
 * When B1 reads 1, app_step must drive LD2 to 1.
 */
void test_app_step_drives_led_high_when_button_reads_high(void)
{
    gpio_read_ExpectAndReturn(B1_PORT, B1_PIN, 1);
    gpio_write_Expect(LD2_PORT, LD2_PIN, 1);

    app_step();
}

/* ----- TODO P3.1 -------------------------------------------------------- *
 * When B1 reads 0, app_step must drive LD2 to 0.
 * Mirror the example with the return value and expected write set to 0.
 */
void test_app_step_drives_led_low_when_button_reads_low(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}

/* ----- TODO P3.2 -------------------------------------------------------- *
 * app_init must, IN THIS ORDER:
 *   1. enable the clock for LD2_PORT
 *   2. enable the clock for B1_PORT
 *   3. configure LD2 as output  (LD2_PORT, LD2_PIN)
 *   4. configure B1 as input    (B1_PORT, B1_PIN)
 * Set the four expectations in that order, then call app_init().
 * Helpers: gpio_enable_clock_Expect(port),
 *          gpio_config_output_Expect(port, pin),
 *          gpio_config_input_Expect(port, pin).
 * Try reordering two expectations and watch the test fail — that is strict
 * ordering catching a wrong initialisation sequence.
 */
void test_app_init_configures_pins_in_order(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}

/* ----- TODO P3.3 (optional / advanced) ---------------------------------- *
 * app_step must forward the raw gpio_read value to gpio_write unchanged.
 * Verify this with a non-zero, non-one value so you prove no filtering occurs.
 *
 * Arrange: gpio_read returns 2 (a value that is neither 0 nor 1)
 * Assert:  gpio_write is called with that same value (2)
 *
 * If this test fails it means app_step applies a threshold or masks the value
 * before writing — a logic bug that the binary tests above cannot detect.
 */
void test_app_step_forwards_raw_value_unchanged(void)
{
    /* YOUR CODE HERE — remove this comment and write the test if you attempt P3.3 */
}
