# Warm-up Exercises — Ceedling, Unity, CMock & CException

Four short exercises that teach the four tools this lab uses, before you test
your own GPIO driver. This folder is a **self-contained Ceedling project**: it
needs only Ruby + Ceedling, no STM32 firmware and no board.

**Estimated time:** 45 minutes total.

> Run every command from **inside this folder** (`docs/exercises/`) in a terminal
> where `ceedling version` works. If it says `command not found`, finish the
> Ruby + Ceedling install first.

```bash
cd docs/exercises
ceedling test:all        # builds and runs every exercise test
```

A passing run ends with `OK`; a failing one prints `FAILED` and the exact file,
line, expected and actual values. Read [`../ceedling_guide.md`](../ceedling_guide.md)
whenever you need a concept refresher.

---

## Exercise 1 — Your first tests with Unity (`test_calculator.c`)

**You will learn:** the anatomy of a test file (`setUp`/`tearDown`, `test_*`
functions), the Arrange-Act-Assert shape, and the common `TEST_ASSERT_*` macros.

The module [`src/calculator.c`](src/calculator.c) is already written. Open
[`test/test_calculator.c`](test/test_calculator.c), run it, then complete TODOs
**EX1.1–EX1.3**.

```bash
ceedling test:test_calculator
```

**Check:** all tests pass; no test is left `IGNORED`. ✓

---

## Exercise 2 — Test-Driven Development (`test_clamp.c`)

**You will learn:** the RED → GREEN → REFACTOR cycle — writing the test first,
watching it fail, then making it pass.

The tests in [`test/test_clamp.c`](test/test_clamp.c) are written for you and
they **fail on purpose** (RED) because `calc_clamp` is not implemented.

1. Run them and watch them fail:

   ```bash
   ceedling test:test_clamp
   ```

2. Implement `calc_clamp` in [`src/calculator.c`](src/calculator.c) (TODO EX2)
   until every test passes (GREEN).

**Check:** all four clamp tests pass. ✓

---

## Exercise 3 — Mocking a dependency with CMock (`test_temperature.c`)

**You will learn:** how to test a module that depends on hardware by replacing
that hardware with an auto-generated **mock**.

[`src/temperature.c`](src/temperature.c) calls `i2c_read_raw()` from
[`src/i2c.h`](src/i2c.h) — there is no `i2c.c` and no sensor. By including
`mock_i2c.h`, CMock gives you a fake `i2c_read_raw` you fully control with
`i2c_read_raw_ExpectAndReturn(...)`.

Open [`test/test_temperature.c`](test/test_temperature.c), study the worked
example, then complete TODOs **EX3.1–EX3.2**.

```bash
ceedling test:test_temperature
```

**Check:** tests pass; try changing an expected register value and watch the
mock make the test fail — proof the call arguments are verified. ✓

---

## Exercise 4 — Exceptions with CException (`test_safe_divide.c`)

**You will learn:** `Try` / `Catch` / `Throw`, and how to test that code throws
the error you expect.

[`src/safe_divide.c`](src/safe_divide.c) throws `ERR_DIVIDE_BY_ZERO` instead of
crashing on a divide-by-zero. Open
[`test/test_safe_divide.c`](test/test_safe_divide.c), study the worked example,
then complete TODO **EX4.1**.

```bash
ceedling test:test_safe_divide
```

**Check:** the happy-path and both throw tests pass. ✓

---

## You are ready for the driver tests

After these exercises you have seen:

| Concept                                              | Exercise |
| ---------------------------------------------------- | -------- |
| Test file anatomy, `setUp`/`tearDown`                | Ex 1     |
| `TEST_ASSERT_*` assertions, Arrange-Act-Assert       | Ex 1     |
| Test-Driven Development (RED → GREEN)                | Ex 2     |
| Mocking a dependency (`mock_*.h`, `ExpectAndReturn`) | Ex 3     |
| Verifying calls and arguments automatically          | Ex 3     |
| `Try` / `Catch` / `Throw` with CException            | Ex 4     |

Return to the [lab README](../../README.md) and start **Phase 2 — Test the GPIO
driver**.
