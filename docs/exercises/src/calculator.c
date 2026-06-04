#include "calculator.h"

/* Exercise 1 — provided and correct. Write tests in test/test_calculator.c. */
int calc_add(int a, int b)
{
    return a + b;
}

int calc_subtract(int a, int b)
{
    return a - b;
}

int calc_multiply(int a, int b)
{
    return a * b;
}

/*
 * Exercise 2 — Test-Driven Development.
 *
 * Right now this returns a wrong, placeholder value, so test/test_clamp.c
 * FAILS (this is the "red" step — that is expected and good).
 *
 * Run:  ceedling test:test_clamp        ← observe RED first
 *
 * TODO EX2: implement calc_clamp so the test passes (the "green" step).
 *   - if value < low  -> return low
 *   - if value > high -> return high
 *   - otherwise       -> return value
 *
 * Run again after your change:  ceedling test:test_clamp   ← expect GREEN
 */
int calc_clamp(int value, int low, int high)
{
    (void)low;
    (void)high;
    return value; /* placeholder — replace with the real logic */
}
