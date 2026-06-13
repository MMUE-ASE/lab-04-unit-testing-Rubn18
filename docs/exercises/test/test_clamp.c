#include "unity.h"
#include "calculator.h"

/*
 * Exercise 2 — Test-Driven Development (TDD).
 *
 * The TDD cycle is: RED -> GREEN -> REFACTOR.
 *   RED      write a test for behaviour that does not exist yet -> it fails.
 *   GREEN    write the smallest code that makes the test pass.
 *   REFACTOR clean up, keeping the test green.
 *
 * The tests below are already written FOR you. Run them first and watch them
 * FAIL — that is the RED step, and it is exactly what should happen:
 *
 *     ceedling test:test_clamp
 *
 * Then open src/calculator.c and implement calc_clamp (TODO EX2) until every
 * test here passes (GREEN). You are not editing this file in Exercise 2.
 */

void setUp(void) {}
void tearDown(void) {}

void test_clamp_returns_value_when_in_range(void)
{
    TEST_ASSERT_EQUAL_INT(5, calc_clamp(5, 0, 10));
}

void test_clamp_floors_value_below_low(void)
{
    TEST_ASSERT_EQUAL_INT(0, calc_clamp(-3, 0, 10));
}

void test_clamp_caps_value_above_high(void)
{
    TEST_ASSERT_EQUAL_INT(10, calc_clamp(42, 0, 10));
}

void test_clamp_handles_value_exactly_on_bounds(void)
{
    TEST_ASSERT_EQUAL_INT(0, calc_clamp(0, 0, 10));
    TEST_ASSERT_EQUAL_INT(10, calc_clamp(10, 0, 10));
}
