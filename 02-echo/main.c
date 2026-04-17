#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

#define LED_PIN 25

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        char symbol = getchar();
        printf(
            "received char: %c [ ASCII code: %d ]\n",
            symbol,
            symbol
        );

        switch(symbol)
        {
            /* Включить LED */
            case 'e':
                gpio_put(LED_PIN, true);
                printf("led enable done\n");
                break;

            /* Выключить LED */
            case 'd':
                gpio_put(LED_PIN, false);
                printf("led disable done\n");
                break;

            /* Версия устройства */
            case 'v':
                printf(
                    "device name: %s, version: %s\n",
                    DEVICE_NAME,
                    DEVICE_VRSN
                );
                break;

            default:
                break;
        }
    }
}