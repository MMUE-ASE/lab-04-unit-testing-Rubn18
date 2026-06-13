#ifndef APP_H
#define APP_H

/*
 * app.h — application logic, separated from main() so it can be unit tested.
 *
 * In Lab 2 the button->LED logic lived inside main()'s while(1) loop, which is
 * impossible to unit test (it never returns). Here that logic is extracted into
 * plain functions that call the GPIO driver. Because they call gpio.h, we can
 * test them by MOCKING gpio.h — no board required. See test/test_app.c.
 */

/* Configure the clocks and pins: LD2 as output, B1 as input. */
void app_init(void);

/* One iteration of the control loop: read B1 and drive LD2 to match. */
void app_step(void);

#endif /* APP_H */
