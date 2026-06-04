#include "safe_divide.h"
#include "CException.h"

/*
 * Provided and correct. In Exercise 4 you write the Catch test for the
 * divide-by-zero path.
 *
 * Throw(code) immediately jumps to the nearest enclosing Catch in the caller.
 * The line `return a / b;` below is never reached when b == 0.
 */
int safe_divide(int a, int b)
{
    if (b == 0) {
        Throw(ERR_DIVIDE_BY_ZERO);
    }
    return a / b;
}
