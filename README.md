# Lab 4 — Unit Testing with Ceedling

[![Hardware](https://img.shields.io/badge/Hardware-STM32_NUCLEO--F412ZG-03234B.svg?logo=stmicroelectronics&logoColor=white)](https://www.st.com/en/evaluation-tools/nucleo-f412zg.html)
[![Tests](https://img.shields.io/badge/Tests-Ceedling-CC0000.svg?logo=ruby&logoColor=white)](https://github.com/ThrowTheSwitch/Ceedling)
[![Framework](https://img.shields.io/badge/Unity-CMock-3DDC84.svg)](https://github.com/ThrowTheSwitch/Unity)
[![GitHub Classroom](https://img.shields.io/badge/GitHub-Classroom-181717.svg?logo=github)](https://classroom.github.com/classrooms/274591709-mmue-arquitectura-sistemas-embebidos-2026)

---

## Table of Contents

- [Context](#context)
- [Objectives](#objectives)
- [Getting Started](#getting-started)
- [Phase Overview](#phase-overview)
- [Phase 0 — Why unit testing](#phase-0--why-unit-testing)
- [Phase 1 — Warm-up: Ceedling, Unity, CMock, CException](#phase-1--warm-up-ceedling-unity-cmock-cexception)
- [Phase 2 — Test your GPIO driver](#phase-2--test-your-gpio-driver)
- [Phase 3 — Mock the driver and measure coverage](#phase-3--mock-the-driver-and-measure-coverage)
- [Milestones](#milestones)
- [CI and submission](#ci-and-submission)
- [Common errors](#common-errors)
- [Rubric](#rubric)

---

## Context

So far the only way you have checked your firmware is by flashing it and watching
the LED. That is slow, manual, and tells you *something* is wrong but never
*where*. Professional embedded teams catch most logic bugs long before the board
is involved, using **unit tests**: small programs that run a single function on
the PC and assert it behaves correctly — in milliseconds, automatically, on every
push.

In this lab you test the GPIO driver you wrote in Labs 1–2 using **Ceedling**,
the standard C unit-test environment, which bundles **Unity** (assertions),
**CMock** (automatic mocks) and **CException** (exceptions). You will test the
driver two ways: directly, against simulated registers; and through a small
application module, by mocking the driver away entirely.

> **Nothing runs on the board in this lab.** Every test compiles and runs on your
> host PC. That is the point — fast, hardware-free feedback on your logic.

---

## Objectives

- Explain what a unit test is and why it matters for embedded development.
- Write tests with Unity: the `test_*`/`setUp` structure, Arrange-Act-Assert,
  and the `TEST_ASSERT_*` macros.
- Use the TDD cycle (RED → GREEN) to drive a small implementation.
- Replace a dependency with a **CMock** mock and verify how it was called.
- Handle and test errors with **CException** (`Try`/`Catch`/`Throw`).
- Test register-level driver code on the host using a fake register block.
- Measure line coverage with **gcov**.

---

## Getting Started

This lab needs **Ruby + Ceedling**. The host C compiler comes with the Ruby
*DevKit*. Full install steps are in the Doc about Software Tools that you have
in the virtual classroom

```bash
# 1 — clone your repository and enter it
git clone https://github.com/<org>/<assigned-repo>.git
cd <assigned-repo>

# 2 — verify the tools answer (Git Bash)
ruby --version
ceedling version

# 3 — run the warm-up exercises immediately (they need no firmware)
make exercises

# 4 — later (Phase 2) carry your completed Lab 2 driver into this workspace
bash scripts/copy_lab2.sh <path-to-your-lab2-repo>
```

Read [`docs/ceedling_guide.md`](docs/ceedling_guide.md) whenever you need a
concept refresher.

---

## Phase Overview

| Phase                                                                  | Goal                                          | Estimated time | Done when                                  |
| ---------------------------------------------------------------------- | --------------------------------------------- | -------------- | ------------------------------------------ |
| [0 — Why unit testing](#phase-0--why-unit-testing)                     | Understand the what and why                   | 15 min         | You can explain a unit test vs. flashing   |
| [1 — Warm-up](#phase-1--warm-up-ceedling-unity-cmock-cexception)       | Unity, TDD, CMock, CException                  | 45 min         | All four exercise suites pass              |
| [2 — Test your driver](#phase-2--test-your-gpio-driver)                | Test `gpio.c` against fake registers           | 45 min         | `test_gpio` passes; TODOs P2.1–P2.4b done  |
| [3 — Mock & coverage](#phase-3--mock-the-driver-and-measure-coverage)  | Mock `gpio.h`, test `app.c`, run gcov          | 35 min         | `test_app` passes; coverage summary shown  |

Total: ~2 h 20 m plus reading. Work the phases in order.

---

## Phase 0 — Why unit testing

**Read [`docs/ceedling_guide.md` §1–§3](docs/ceedling_guide.md#1-why-unit-testing).**
Before writing any test, make sure you can answer:

- What does a unit test check, and how is that different from flashing the board?
- What is the **Arrange-Act-Assert** shape of a test?
- What do `setUp` and `tearDown` do, and why does each test get a clean slate?

This phase is reading only — it gives the vocabulary the rest of the lab uses.

---

## Phase 1 — Warm-up: Ceedling, Unity, CMock, CException

**Before touching your driver**, complete the four exercises in
[`docs/exercises/`](docs/exercises/README.md). They are self-contained (no
firmware needed) and cover every tool this lab uses.

```bash
make exercises          # runs all four; or cd docs/exercises && ceedling test:all
```

| Exercise | You learn                                          | File                                   |
| -------- | -------------------------------------------------- | -------------------------------------- |
| Ex 1     | Unity assertions, test anatomy, AAA                | `docs/exercises/test/test_calculator.c`|
| Ex 2     | Test-Driven Development (RED → GREEN)              | `docs/exercises/test/test_clamp.c`     |
| Ex 3     | Mocking a dependency with CMock                    | `docs/exercises/test/test_temperature.c`|
| Ex 4     | Exceptions with CException (`Try`/`Catch`/`Throw`) | `docs/exercises/test/test_safe_divide.c`|

**Check (M1):** every exercise suite passes with no `IGNORED` tests. ✓

---

## Phase 2 — Test your GPIO driver

Now bring in the driver you wrote and test it for real. Lab 4 ships the tests but
not the driver — you test *your* code.

```bash
bash scripts/copy_lab2.sh <path-to-your-lab2-repo>   # copies src/ and inc/ in
make test                                            # builds + runs all tests
```

Open [`test/test_gpio.c`](test/test_gpio.c). It tests `gpio.c` directly: instead
of a real peripheral address, the driver is handed `fake_gpio_port()` — a block
of ordinary memory provided by
[`test/support/fake_registers.h`](test/support/fake_registers.h) — and you check
what the driver wrote with the `FAKE_MODER`/`FAKE_IDR`/`FAKE_BSRR` accessors.

Study the worked example, then complete the TODOs:

| TODO   | Test                                                                |
| ------ | ------------------------------------------------------------------- |
| P2.1   | `gpio_config_input` sets the pin's MODER field to `00`              |
| P2.2   | configuring one pin does not corrupt a neighbour's bits             |
| P2.3   | `gpio_read` returns the pin level from IDR                          |
| P2.4a  | `gpio_write` with value ≠ 0 writes `(1 << pin)` to BSRR            |
| P2.4b  | `gpio_write` with value = 0 writes `(1 << (pin + 16))` to BSRR     |

> **Why fake registers?** Your driver writes to addresses like `base + 0x14`. On
> the chip those are registers; on a PC they are nothing. Pointing the driver at
> real memory lets the exact same driver code run on the host. See
> [`docs/ceedling_guide.md` §7](docs/ceedling_guide.md#7-testing-register-level-driver-code).

**Check (M2):** `make test` shows `test_gpio` passing, all four TODOs done. ✓

---

## Phase 3 — Mock the driver and measure coverage

Code that *uses* the driver should not re-test the driver — it should assume the
driver works and check its own logic. That is what mocking is for.

[`src/app.c`](src/app.c) (provided) holds the button→LED logic, extracted from
`main()` so it can be tested. Open [`test/test_app.c`](test/test_app.c): it
includes `mock_gpio.h`, so every `gpio_*` call is a mock you control and verify.

| TODO              | Test                                                                |
| ----------------- | ------------------------------------------------------------------- |
| P3.1              | `app_step` drives LD2 low when B1 reads low                         |
| P3.2              | `app_init` configures the clocks and pins in the correct order      |
| P3.3 *(optional)* | `app_step` forwards the raw read value without filtering            |

Then measure how much of your driver the tests exercise:

```bash
make coverage          # runs tests with gcov and prints a coverage summary
```

Aim to cover the pin-level functions of `gpio.c` — `config_input`,
`config_output`, `read`, `write`, including both sides of each `if`.
`gpio_enable_clock` stays uncovered on purpose: it targets the real RCC register
and cannot run against a fake port (see
[Phase 2](#phase-2--test-your-gpio-driver)), so do not chase 100% — covering the
four pin functions is the goal. See
[`docs/ceedling_guide.md` §8](docs/ceedling_guide.md#8-coverage-with-gcov).

**Check (M3, M4):** `test_app` passes; `make coverage` prints a per-file summary
showing every pin-level path in `gpio.c` exercised. ✓

---

## Milestones

| Milestone                          | How to verify                                                        |
| ---------------------------------- | ------------------------------------------------------------------- |
| M1 — Warm-ups pass                 | `make exercises` → all four suites `OK`, no `IGNORED`               |
| M2 — Driver tested                 | `make test` → `test_gpio` passes; TODOs P2.1–P2.4b implemented       |
| M3 — App logic tested with mocks   | `make test` → `test_app` passes; TODOs P3.1–P3.2 implemented         |
| M4 — Coverage measured             | `make coverage` runs and reports line coverage for `gpio.c`/`app.c`  |
| M5 — All green in CI               | The **Lab 4 — Unit Tests** workflow passes on your last push         |

---

## CI and submission

Every `push` triggers the **Lab 4 — Unit Tests** workflow, which:

```text
gem install ceedling                       →  installs the test environment
ceedling test:all   (in docs/exercises/)   →  runs the warm-up suites (always)
ceedling test:all                          →  runs test_gpio + test_app, once your
                                              Lab 2 driver has been carried in
ceedling gcov:all                          →  prints the coverage summary
```

The warm-up step goes green once Phase 1 is done. The driver/app step goes green
once you have carried your Lab 2 sources in with `copy_lab2.sh` **and committed
them**, and your TODOs pass. Results appear in the **Actions** tab. Submission is
the last commit pushed before the deadline.

### Commit conventions

```text
test(lab4): complete Unity assertions in warm-up exercise 1
test(lab4): add fake-register tests for gpio_read and gpio_write
test(lab4): mock gpio.h to verify app_step and app_init
docs(lab4): carry Lab 2 driver into the workspace
```

---

## Common errors

| Symptom                                          | Likely cause                                                                |
| ------------------------------------------------ | --------------------------------------------------------------------------- |
| `ceedling: command not found`                    | Ruby/Ceedling not installed or not on PATH — see `Docs/software_tools.md` §9 |
| `make: command not found`                        | Use the Git Bash terminal, or run the `ceedling` commands directly          |
| `gpio.h: No such file` on `make test`            | Driver not carried in yet — run `bash scripts/copy_lab2.sh <path>` and commit |
| Test reported `IGNORED`                          | A `TEST_IGNORE*` line is still in the test — remove it after you write it    |
| `Expected 0x... but was 0x...` on a MODER test   | Wrong bit position — each pin owns 2 bits starting at `pin * 2`              |
| `gpio_read Expected to be called but was not`    | `app.c` did not call the mock you set up, or in the wrong order             |
| Assertion failure inside `fake_registers.c`      | The host placed test memory above 4 GB — report it; the fake allocator needs a sub-4 GB region |
| Coverage step complains about `gcovr`            | Optional HTML report needs `pip install gcovr`; the console summary does not |

---

## Rubric

| Criterion                                                          | Weight |
| ----------------------------------------------------------------- | -----: |
| Warm-up exercises complete and passing (M1)                       |    20% |
| GPIO driver tests P2.1–P2.4b correct and passing (M2)             |    30% |
| App logic tested with CMock, P3.1–P3.2 (M3); P3.3 adds no points |    25% |
| Coverage run and driver paths exercised (M4)                      |    10% |
| Tests are meaningful (clear Arrange-Act-Assert, real assertions)  |     5% |
| CI green on final push (M5)                                       |     5% |
| Commit quality — Conventional Commits                             |     5% |
