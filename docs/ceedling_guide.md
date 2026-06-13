# Ceedling Guide — Unit Testing in C

A concept reference for Lab 4. Read [section 1](#1-why-unit-testing) before the
warm-up exercises; use the rest as a lookup while you work.

## Table of Contents

1. [Why unit testing](#1-why-unit-testing)
2. [The four tools: Ceedling, Unity, CMock, CException](#2-the-four-tools)
3. [Anatomy of a test](#3-anatomy-of-a-test)
4. [Unity assertions cheat sheet](#4-unity-assertions-cheat-sheet)
5. [Mocking with CMock](#5-mocking-with-cmock)
6. [Exceptions with CException](#6-exceptions-with-cexception)
7. [Testing register-level driver code](#7-testing-register-level-driver-code)
8. [Coverage with gcov](#8-coverage-with-gcov)
9. [Command reference](#9-command-reference)
10. [Common errors](#10-common-errors)
11. [Further reading](#11-further-reading)

---

## 1. Why unit testing

A **unit test** runs one small piece of your code in isolation and checks it
does what you claim — automatically, in milliseconds, on your PC. Compare that
to the only check you have had so far: flash the board, press the button, watch
the LED, and *guess* why it did not work.

Benefits that matter for embedded work:

- **No hardware needed.** Tests run on the host, so you can develop logic before
  the board is even on your desk, and CI can verify it on a server.
- **Fast feedback.** A whole suite runs in under a second; you find a broken edge
  case immediately, not after a 30-second flash cycle.
- **Regression safety.** Once a behaviour is captured in a test, you will know
  the instant a later change breaks it.
- **Better design.** Code that is easy to test is usually code with clear
  boundaries — which is good code regardless of testing.

The cost is that you write the logic *and* the tests. The trade pays off quickly
for anything you will touch more than once.

---

## 2. The four tools

| Tool           | Role                                                                 | Official docs |
| -------------- | -------------------------------------------------------------------- | ------------- |
| **Ceedling**   | The build/test runner. Finds your `test_*.c` files, builds each into a tiny program with its own runner, executes them, and reports results. Configured by `project.yml`. | [GitHub](https://github.com/ThrowTheSwitch/Ceedling) · [docs site](https://throwtheswitch.github.io/Ceedling/) |
| **Unity**      | The assertion library. Provides `TEST_ASSERT_*` macros and the pass/fail machinery. | [GitHub](https://github.com/ThrowTheSwitch/Unity) · [assertions reference](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md) |
| **CMock**      | Generates fake ("mock") implementations of a dependency from its header, so you can test code in isolation and verify how it was called. | [GitHub](https://github.com/ThrowTheSwitch/CMock) · [CMock_Summary.md](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md) |
| **CException** | Adds `Try`/`Catch`/`Throw` exception handling to C. | [GitHub](https://github.com/ThrowTheSwitch/CException) |

Ceedling bundles all four. You enable CMock and CException in `project.yml`:

```yaml
:project:
  :use_mocks: TRUE        # CMock
  :use_exceptions: TRUE   # CException
```

---

## 3. Anatomy of a test

```c
#include "unity.h"          // assertion macros
#include "calculator.h"     // the code under test

void setUp(void)    { /* runs before EACH test */ }
void tearDown(void) { /* runs after  EACH test */ }

void test_calc_add_two_positive_numbers(void)
{
    int result = calc_add(2, 3);          // Act
    TEST_ASSERT_EQUAL_INT(5, result);     // Assert
}
```

- Every test is a `void test_*(void)` function. Ceedling discovers them
  automatically — you never maintain a list or a `main()`.
- `setUp`/`tearDown` run around *every* test, giving each a clean slate.
- Structure each test as **Arrange → Act → Assert** (AAA): set up inputs, call
  the function, check the result. One behaviour per test reads best.

---

## 4. Unity assertions cheat sheet

| Macro                                          | Checks                          |
| ---------------------------------------------- | ------------------------------- |
| `TEST_ASSERT_TRUE(c)` / `TEST_ASSERT_FALSE(c)` | a condition                     |
| `TEST_ASSERT_EQUAL_INT(exp, act)`              | signed integers equal           |
| `TEST_ASSERT_EQUAL_UINT(exp, act)`             | unsigned integers equal         |
| `TEST_ASSERT_EQUAL_HEX32(exp, act)`            | 32-bit values, shown in hex     |
| `TEST_ASSERT_EQUAL_PTR(exp, act)`              | pointers equal                  |
| `TEST_ASSERT_NULL(p)` / `TEST_ASSERT_NOT_NULL(p)` | null-ness                    |
| `TEST_ASSERT_BITS(mask, exp, act)`             | only the masked bits            |
| `TEST_ASSERT_EQUAL_INT_ARRAY(exp, act, n)`     | arrays element-by-element       |
| `TEST_FAIL_MESSAGE("why")`                     | force a failure                 |
| `TEST_IGNORE_MESSAGE("why")`                   | skip (reported as IGNORED)      |

`HEX32` is your friend for register tests: a mismatch prints both values in hex,
so a wrong bit is obvious. Add a message to any assert with the `_MESSAGE`
variant, e.g. `TEST_ASSERT_EQUAL_INT_MESSAGE(5, r, "add(2,3)")`.

---

## 5. Mocking with CMock

When the code under test calls a dependency you cannot or do not want to run
(hardware, I2C, another module), you **mock** it. For each function in the
mocked header, CMock generates helpers:

| Helper                                  | Meaning                                              |
| --------------------------------------- | ---------------------------------------------------- |
| `foo_Expect(args)`                      | expect one call to `foo(args)` (void return)         |
| `foo_ExpectAndReturn(args, ret)`        | expect one call to `foo(args)`; make it return `ret` |
| `foo_Ignore()`                          | allow any number of calls; ignore args and count     |
| `foo_IgnoreAndReturn(ret)`             | like `Ignore` but makes every call return `ret`      |
| `foo_ExpectAnyArgs()`                   | expect a call but do not check the arguments         |
| `foo_AddCallback(fn)`                   | run your own function when `foo` is called           |

To use them, include the **mock** header instead of the real one:

```c
#include "mock_i2c.h"     // NOT "i2c.h"

void test_reads_sensor(void)
{
    i2c_read_raw_ExpectAndReturn(0x07, 65);   // arrange the fake
    TEST_ASSERT_EQUAL_INT(25, temperature_read_celsius());
}
```

After the test, Ceedling **verifies** every expectation: each expected call must
happen, with the arguments you specified, the right number of times. A missing
or extra call fails the test. With strict ordering enabled (as in this lab),
calls must also happen in the order you declared — handy for checking an
initialisation sequence.

---

## 6. Exceptions with CException

C has no `try`/`catch`. CException adds it with `setjmp`/`longjmp` under the
hood:

```c
#include "CException.h"

CEXCEPTION_T e = 0;
Try {
    safe_divide(1, 0);                 // Throws ERR_DIVIDE_BY_ZERO
    TEST_FAIL_MESSAGE("expected throw");
} Catch (e) {
    TEST_ASSERT_EQUAL(ERR_DIVIDE_BY_ZERO, e);
}
```

- `Throw(code)` jumps straight to the nearest enclosing `Catch`, skipping the
  rest of the `Try` block.
- The `TEST_FAIL` after the throwing call is the safety net: if nothing throws,
  `Catch` is skipped and the test fails as it should.
- `CEXCEPTION_T` is the error-code type (an `int` by default).

---

## 7. Testing register-level driver code

Your GPIO driver writes to fixed addresses such as `base + 0x14`. On the chip
those are registers; on your PC they are nothing, and dereferencing them would
crash. Two strategies:

1. **Fake registers (used for `gpio.c` in this lab).** Give the driver the
   address of a real block of memory instead of a peripheral address, then read
   that memory back to see what the driver wrote.
   `test/support/fake_registers.c` provides this block via `fake_gpio_port()`
   and the `FAKE_MODER`/`FAKE_IDR`/`FAKE_BSRR` accessors. Because the driver
   takes the base as a `uint32_t`, the block is deliberately allocated below the
   4 GB line so the address survives the round-trip on a 64-bit PC.

2. **Mock the driver (used for `app.c` in this lab).** Code that *uses* the
   driver should not re-test the driver. Mock `gpio.h` and verify the calls.

A function like `gpio_enable_clock` that hard-codes peripheral constants
(`GPIOB_BASE`) and touches a fixed `RCC` register fits neither cleanly — it is
not parameterised by its target. That is a hint about **testable design**:
functions that receive their dependencies as parameters are easy to test;
functions that reach out to globals are not.

---

## 8. Coverage with gcov

Coverage measures which lines your tests actually executed. Run:

```bash
make coverage          # or: ceedling gcov:all
```

Ceedling rebuilds the tests instrumented with `gcov` and prints a per-file
line-coverage summary. Aim to exercise every branch of your driver — the
config, read and write paths, and both sides of each `if`.

> **Optional HTML report.** A browsable report needs `gcovr`
> (`pip install gcovr`). With it installed you can enable HTML output in
> `project.yml` under `:gcov: :reports:`. The console summary needs only the
> `gcov` that ships with GCC.

Coverage tells you what was *run*, not whether it was *correct* — 100% coverage
with weak assertions still proves little. Use it to find untested code, not as a
goal in itself.

---

## 9. Command reference

| Command                         | Does                                               |
| ------------------------------- | -------------------------------------------------- |
| `make test`                     | build + run all tests (`ceedling test:all`)        |
| `make coverage`                 | run tests with coverage (`ceedling gcov:all`)      |
| `make exercises`                | run the warm-up suite in `docs/exercises/`         |
| `make clean`                    | delete generated files (`ceedling clobber`)        |
| `ceedling test:test_gpio`       | run a single test file                             |
| `ceedling test:all`             | run every test                                     |
| `ceedling clobber`              | remove the whole `build/` directory                |
| `ceedling version`              | print tool versions                                |

---

## 10. Common errors

| Symptom                                              | Likely cause                                                                 |
| ---------------------------------------------------- | ---------------------------------------------------------------------------- |
| `ceedling: command not found`                        | Ruby/Ceedling not installed or not on PATH — see `Docs/software_tools.md` §9 |
| `cannot load such file -- ...` / Ruby errors         | Run the command from the folder that contains `project.yml`                  |
| `mock_gpio.h: No such file`                          | You included the real `gpio.h`; include `mock_gpio.h`, and ensure `:use_mocks: TRUE` |
| `undefined reference to gpio_read`                   | Mocked and real driver both linked — a test should include the mock *or* link the real `.c`, not both |
| Test `IGNORED`                                       | A `TEST_IGNORE*` is still in the test body — remove it once you write the test |
| `Expected ... but was ...` on a register test        | Wrong bit position or mask — remember each pin owns 2 bits at `pin * 2` in MODER |
| `... Expected to be called but was not`              | Your code never made a call the mock expected (or made them out of order)    |
| `make: command not found`                            | Use the Git Bash terminal; or run the `ceedling` command directly            |
| Coverage report step errors about `gcovr`            | `gcovr` not installed — use the console summary, or `pip install gcovr`      |

---

## 11. Further reading

Go here when the guide above leaves a gap or you want to explore a feature not
covered in the lab. The GitHub repos are more up-to-date than the marketing
pages and contain the full documentation under their `docs/` folder.

### Ceedling (v1.0.1 · January 2025)

| Resource | Link |
| -------- | ---- |
| GitHub repository | [ThrowTheSwitch/Ceedling](https://github.com/ThrowTheSwitch/Ceedling) |
| Official docs site | [throwtheswitch.github.io/Ceedling](https://throwtheswitch.github.io/Ceedling/) |
| `project.yml` full reference | [CeedlingPacket.md](https://github.com/ThrowTheSwitch/Ceedling/blob/master/docs/CeedlingPacket.md) |

### Unity (v2.6.1 · January 2025)

| Resource | Link |
| -------- | ---- |
| GitHub repository | [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity) |
| Getting started guide | [UnityGettingStartedGuide.md](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md) |
| Full assertion reference | [UnityAssertionsReference.md](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md) |

### CMock (v2.6.0 · January 2025)

| Resource | Link |
| -------- | ---- |
| GitHub repository | [ThrowTheSwitch/CMock](https://github.com/ThrowTheSwitch/CMock) |
| All mock helpers with examples | [CMock_Summary.md](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md) |
| Known issues | [CMockKnownIssues.md](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMockKnownIssues.md) |

### CException

| Resource | Link |
| -------- | ---- |
| GitHub repository | [ThrowTheSwitch/CException](https://github.com/ThrowTheSwitch/CException) |

The two most useful references for this lab are the **Ceedling docs site**
(covers all `project.yml` options and plugins) and **CMock_Summary.md** (lists
every generated mock helper — `_Expect`, `_IgnoreAndReturn`, `_AddCallback`,
etc. — with examples).
