#include "unity.h"
#include "gpio.h"
#include "fake_registers.h"

/*
 * Phase 2 — Test your GPIO driver directly.
 *
 * Here gpio.c is NOT mocked: it is the real code under test. The trick is the
 * port base. Instead of a real peripheral address, we pass fake_gpio_port() —
 * the address of a block of ordinary memory (see test/support/fake_registers.h).
 * The driver reads and writes that block exactly as if it were hardware, and we
 * inspect it with the FAKE_* macros.
 *
 *   FAKE_MODER(port)  the mode register the driver configures
 *   FAKE_IDR(port)    the input register; set it to simulate a pin level
 *   FAKE_BSRR(port)   the set/reset register the driver writes to drive a pin
 *
 * Each pin owns 2 bits in MODER: the field for pin N starts at bit (N * 2).
 * Run it:  make test    (or: ceedling test:test_gpio)
 */

static uint32_t port;

void setUp(void)
{
    fake_registers_reset();   /* every test starts with zeroed registers */
    port = fake_gpio_port();
}

void tearDown(void) {}

/* ----- Worked example: read it, then run it ----------------------------- *
 * gpio_config_output must set the pin's 2-bit MODER field to 01 (output).
 */
void test_config_output_sets_moder_to_01(void)
{
    gpio_config_output(port, 5);

    /* bits [11:10] for pin 5 must read 01 */
    uint32_t field = (FAKE_MODER(port) >> (5 * 2)) & 0x3u;
    TEST_ASSERT_EQUAL_HEX32(0x1u, field);
}

/* ----- TODO P2.1 -------------------------------------------------------- *
 * gpio_config_input must set the pin's MODER field to 00 (input).
 * Arrange: pre-set the field to a non-zero value so you prove it gets cleared:
 *     FAKE_MODER(port) = 0x3u << (9 * 2);   // pin 9 currently 11 (analog)
 * Act:     gpio_config_input(port, 9);
 * Assert:  the field for pin 9 now reads 0x0.
 */
void test_config_input_sets_moder_to_00(void)
{
    /* YOUR CODE HERE */
    FAKE_MODER(port) = 0x3u << (9 * 2); // Escribimos un valor distinto de 00 p.e. 11 para la comprobacion posterior
    gpio_config_input(port,9);
    uint32_t field = (FAKE_MODER(port) >> (9*2)) & 0x3u;

    TEST_ASSERT_EQUAL_HEX32(0x0u, field);
}

/* ----- TODO P2.2 -------------------------------------------------------- *
 * Configuring one pin must NOT disturb a neighbour's bits.
 * Arrange: gpio_config_output(port, 0);
 * Act:     gpio_config_output(port, 1);
 * Assert:  pin 0's field still reads 01 AND pin 1's field reads 01.
 */
void test_config_output_does_not_corrupt_other_pins(void)
{
    /* YOUR CODE HERE */
    gpio_config_output(port, 0);
    gpio_config_output(port, 1);
    uint32_t field0 = (FAKE_MODER(port) >> (0 * 2)) & 0x3u;
    uint32_t field1 = (FAKE_MODER(port) >> (1 * 2)) & 0x3u;
    TEST_ASSERT_EQUAL_HEX32(0x1u, field0);
    TEST_ASSERT_EQUAL_HEX32(0x1u, field1);
}

/* ----- TODO P2.3 -------------------------------------------------------- *
 * gpio_read returns the level of the pin from IDR.
 * Arrange: FAKE_IDR(port) = (1u << 13);   // simulate pin 13 high
 * Assert:  gpio_read(port, 13) == 1  AND  gpio_read(port, 12) == 0.
 */
void test_read_returns_pin_level_from_idr(void)
{
    /* YOUR CODE HERE */
    FAKE_IDR(port) = (1u << 13); 
    TEST_ASSERT_EQUAL_INT(1, gpio_read(port, 13));
    TEST_ASSERT_EQUAL_INT(0, gpio_read(port, 12));
}

/* ----- TODO P2.4a ------------------------------------------------------- *
 * gpio_write with value != 0 must write (1 << pin) to BSRR (set / high).
 * Act:    gpio_write(port, 7, 1);
 * Assert: FAKE_BSRR(port) == (1u << 7).
 */
void test_write_drives_pin_high_via_bsrr(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}

/* ----- TODO P2.4b ------------------------------------------------------- *
 * gpio_write with value == 0 must write (1 << (pin + 16)) to BSRR (reset).
 * Note: fake_registers_reset() zeroes the register CONTENTS — the port
 * address stays the same and you do not need to call fake_gpio_port() again.
 * Act:    gpio_write(port, 7, 0);
 * Assert: FAKE_BSRR(port) == (1u << (7 + 16)).
 */
void test_write_drives_pin_low_via_bsrr(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}

/*
 * Note — gpio_enable_clock is intentionally NOT tested here. It compares the
 * port base against the real GPIOB_BASE/GPIOC_BASE constants and writes the
 * real RCC register, so it cannot run against a fake port. Making a function
 * like that host-testable is a design topic discussed in docs/ceedling_guide.md.
 */
