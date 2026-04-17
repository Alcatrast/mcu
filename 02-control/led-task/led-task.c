#include "led-task.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <stdint.h>

/* параметры */

const uint LED_PIN = 25;

/* период в микросекундах */

uint LED_BLINK_PERIOD_US = 500000;

/* состояние */

uint64_t led_ts;

led_state_t led_state;

/* init */

void led_task_init()
{
    led_state = LED_STATE_OFF;

    led_ts = 0;

    gpio_init(LED_PIN);

    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, false);
}

/* handler */

void led_task_handle()
{
    switch (led_state)
    {
        case LED_STATE_OFF:

            gpio_put(LED_PIN, false);

            break;

        case LED_STATE_ON:

            gpio_put(LED_PIN, true);

            break;

        case LED_STATE_BLINK:

            if (time_us_64() > led_ts)
            {
                led_ts =
                    time_us_64()
                    + (LED_BLINK_PERIOD_US / 2);

                gpio_xor_mask(
                    1u << LED_PIN
                );
            }

            break;

        default:

            break;
    }
}

/* set state */

void led_task_state_set(
    led_state_t state
)
{
    led_state = state;
}

/* НОВАЯ функция */

void led_task_set_blink_period_ms(
    uint32_t period_ms
)
{
    LED_BLINK_PERIOD_US =
        period_ms * 1000;

    printf(
        "blink period set to %u ms\n",
        period_ms
    );
}