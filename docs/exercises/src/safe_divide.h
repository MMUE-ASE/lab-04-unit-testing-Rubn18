#ifndef SAFE_DIVIDE_H
#define SAFE_DIVIDE_H

/*
 * safe_divide.h — the module under test in Exercise 4 (CException).
 *
 * C has no built-in exceptions. CException adds them: a function deep in the
 * call stack can Throw an error code, and a caller wrapping the call in
 * Try { ... } Catch(e) { ... } receives it — no return-code plumbing through
 * every layer.
 */

/* Error code thrown when dividing by zero. */
#define ERR_DIVIDE_BY_ZERO 0x10

/* Returns a / b. Throws ERR_DIVIDE_BY_ZERO if b == 0 instead of crashing. */
int safe_divide(int a, int b);

#endif /* SAFE_DIVIDE_H */
