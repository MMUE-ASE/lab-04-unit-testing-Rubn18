#ifndef FAKE_REGISTERS_H
#define FAKE_REGISTERS_H

#include <stdint.h>

/*
 * fake_registers.h — a software stand-in for a GPIO peripheral, for host tests.
 *
 * Why this exists
 * ---------------
 * Your driver reaches the hardware by writing to fixed addresses, e.g.
 *     GPIOx_MODER(base) == *(volatile uint32_t *)(base + 0x00)
 * On the STM32 those addresses are real registers. On your PC they are not —
 * dereferencing 0x40020400 would crash. So instead of a real port, the tests
 * hand the driver the address of a small block of ordinary memory and then
 * inspect that memory to see what the driver wrote.
 *
 * The driver takes the port as a uint32_t. On a 64-bit PC a normal address does
 * not fit in 32 bits, so fake_registers.c carefully allocates this block below
 * the 4 GB line. You do not need to worry about that — just use the helpers.
 *
 * How you use it (see test/test_gpio.c)
 * -------------------------------------
 *     void setUp(void) { fake_registers_reset(); }   // fresh registers each test
 *
 *     uint32_t port = fake_gpio_port();               // pass this as the base
 *     gpio_config_output(port, 7);
 *     uint32_t field = (FAKE_MODER(port) >> (7 * 2)) & 0x3u;
 *     TEST_ASSERT_EQUAL_HEX32(0x1u, field);          // 01 = output mode
 */

/* Base address of the fake GPIO port. Pass this where the driver wants a port
 * base. Stable for the whole test program. */
uint32_t fake_gpio_port(void);

/* Zero every fake register. Call this in setUp() so each test starts clean. */
void fake_registers_reset(void);

/* Direct access to a fake register, by the same offsets the driver uses
 * (RM0402 GPIO register map). Use the LHS to arrange inputs (e.g. IDR) and to
 * assert on outputs (e.g. MODER, BSRR). */
#define FAKE_REG(base, offset) (*(volatile uint32_t *)((uintptr_t)(base) + (offset)))

#define FAKE_MODER(base)   FAKE_REG((base), 0x00U) /* pin mode (2 bits/pin)   */
#define FAKE_OTYPER(base)  FAKE_REG((base), 0x04U) /* output type             */
#define FAKE_OSPEEDR(base) FAKE_REG((base), 0x08U) /* output speed            */
#define FAKE_PUPDR(base)   FAKE_REG((base), 0x0CU) /* pull-up/pull-down       */
#define FAKE_IDR(base)     FAKE_REG((base), 0x10U) /* input data (read)       */
#define FAKE_ODR(base)     FAKE_REG((base), 0x14U) /* output data             */
#define FAKE_BSRR(base)    FAKE_REG((base), 0x18U) /* bit set/reset (write)   */

#endif /* FAKE_REGISTERS_H */
