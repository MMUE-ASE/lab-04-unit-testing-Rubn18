#include "app.h"
#include "board.h"
#include "gpio.h"

/*
 * Provided and correct. This is the module UNDER TEST in Phase 3: you write
 * test/test_app.c, which mocks every gpio_* call so you can verify this logic
 * without hardware.
 *
 * The behaviour mirrors the Lab 2 firmware: LD2 follows B1.
 */

void app_init(void)
{
    gpio_enable_clock(LD2_PORT);
    gpio_enable_clock(B1_PORT);
    gpio_config_output(LD2_PORT, LD2_PIN);
    gpio_config_input(B1_PORT, B1_PIN);
}

void app_step(void)
{
    int pressed = gpio_read(B1_PORT, B1_PIN);
    gpio_write(LD2_PORT, LD2_PIN, pressed);
}
