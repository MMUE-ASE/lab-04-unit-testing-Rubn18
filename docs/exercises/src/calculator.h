#ifndef CALCULATOR_H
#define CALCULATOR_H

/*
 * calculator.h — a tiny pure-function module for Exercises 1 and 2.
 *
 * "Pure" means: output depends only on the inputs, with no hardware, no
 * globals, no side effects. Pure functions are the easiest thing to unit test,
 * so they are where everyone starts.
 */

/* Exercise 1 — already implemented. You write tests for these. */
int calc_add(int a, int b);
int calc_subtract(int a, int b);
int calc_multiply(int a, int b);

/* Exercise 2 (TDD) — NOT implemented yet. A failing test already exists in
 * test/test_clamp.c; your job is to implement this until the test passes.
 *
 * clamp(value, low, high) returns:
 *   low   if value < low
 *   high  if value > high
 *   value otherwise
 */
int calc_clamp(int value, int low, int high);

#endif /* CALCULATOR_H */
