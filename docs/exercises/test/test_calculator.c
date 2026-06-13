#include "unity.h"
#include "calculator.h"

/*
 * Exercise 1 — Your first Unity tests.
 *
 * Anatomy of a test file:
 *   - #include "unity.h" and the header of the code you test.
 *   - setUp()  runs before EVERY test  (fresh state).
 *   - tearDown() runs after EVERY test  (cleanup).
 *   - each test is a void function named test_*  — Ceedling finds them
 *     automatically, you never maintain a list.
 *
 * Run it:  ceedling test:test_calculator
 *
 * A test follows the Arrange-Act-Assert (AAA) shape:
 *   Arrange — set up inputs
 *   Act     — call the function under test
 *   Assert  — check the result with a TEST_ASSERT_* macro
 */

void setUp(void) {}
void tearDown(void) {}

/* ----- Worked example: read it, then run it ----------------------------- */
void test_calc_add_two_positive_numbers(void)
{
    /* Arrange + Act */
    int result = calc_add(2, 3);

    /* Assert */
    TEST_ASSERT_EQUAL_INT(5, result);
}

/* ----- TODO EX1.1 ------------------------------------------------------- *
 * Assert that calc_add(-4, 4) == 0.
 * Use TEST_ASSERT_EQUAL_INT(expected, actual).
 */
void test_calc_add_cancels_to_zero(void)
{
    /* YOUR CODE HERE */
    TEST_ASSERT_EQUAL_INT(0,calc_add(-4,4));
}

/* ----- TODO EX1.2 ------------------------------------------------------- *
 * Test calc_subtract: assert calc_subtract(10, 3) == 7.
 */
void test_calc_subtract_basic(void)
{
    /* YOUR CODE HERE */
    TEST_ASSERT_EQUAL_INT(7, calc_subtract(10, 3));
}

/* ----- TODO EX1.3 ------------------------------------------------------- *
 * Test calc_multiply with a negative operand:
 *   assert calc_multiply(-6, 7) == -42.
 */
void test_calc_multiply_negative(void)
{
    /* YOUR CODE HERE */
    TEST_ASSERT_EQUAL_INT(-42, calc_multiply(-6, 7));
}
