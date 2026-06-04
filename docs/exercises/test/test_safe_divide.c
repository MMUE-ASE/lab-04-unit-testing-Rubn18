#include "unity.h"
#include "safe_divide.h"
#include "CException.h"

/*
 * Exercise 4 — Exceptions in C with CException.
 *
 * To test that code throws, you wrap the call in Try/Catch yourself:
 *
 *     CEXCEPTION_T e = 0;
 *     Try {
 *         ... call that should throw ...
 *         TEST_FAIL_MESSAGE("expected an exception but none was thrown");
 *     } Catch (e) {
 *         TEST_ASSERT_EQUAL(EXPECTED_CODE, e);
 *     }
 *
 * The TEST_FAIL inside Try is the safety net: if no exception is thrown, the
 * Catch block is skipped and the test correctly fails.
 *
 * Run it:  ceedling test:test_safe_divide
 */

void setUp(void) {}
void tearDown(void) {}

/* ----- Happy path: no exception ----------------------------------------- */
void test_safe_divide_normal(void)
{
    TEST_ASSERT_EQUAL_INT(4, safe_divide(8, 2));
}

/* ----- Worked example: the exception path ------------------------------- */
void test_safe_divide_by_zero_throws(void)
{
    CEXCEPTION_T e = 0;

    Try {
        safe_divide(1, 0);
        TEST_FAIL_MESSAGE("safe_divide(1, 0) should have thrown");
    } Catch (e) {
        TEST_ASSERT_EQUAL(ERR_DIVIDE_BY_ZERO, e);
    }
}

/* ----- TODO EX4.1 ------------------------------------------------------- *
 * Prove the SAME exception code is thrown for a different numerator, e.g.
 * safe_divide(-50, 0). Copy the pattern above and assert e == ERR_DIVIDE_BY_ZERO.
 */
void test_safe_divide_negative_by_zero_throws(void)
{
    /* YOUR CODE HERE */
    TEST_IGNORE_MESSAGE("Remove this line and write the test");
}
