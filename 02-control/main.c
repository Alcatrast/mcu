#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "RP2040"
#define DEVICE_VRSN "1.0"

/* ===== CALLBACKS ===== */

void version_callback(const char* args)
{
    printf(
        "device name: '%s', firmware version: %s\n",
        DEVICE_NAME,
        DEVICE_VRSN
    );
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
}

void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
}

void led_blink_set_period_ms_callback(const char* args)
{
    uint32_t period_ms = 0;

    sscanf(args, "%u", &period_ms);

    if (period_ms == 0)
    {
        printf("error: period must be > 0\n");
        return;
    }

    led_task_set_blink_period_ms(period_ms);

    printf(
        "blink period set to %u ms\n",
        period_ms
    );
}

void mem_callback(const char* args)
{
    uint32_t address = 0;

    sscanf(args, "%x", &address);

    if (address == 0)
    {
        printf("error: invalid address\n");
        return;
    }

    uint32_t value =
        *((volatile uint32_t*)address);

    printf(
        "mem[0x%08X] = 0x%08X (%u)\n",
        address,
        value,
        value
    );
}

void wmem_callback(const char* args)
{
    uint32_t address = 0;
    uint32_t value = 0;

    int parsed =
        sscanf(
            args,
            "%x %x",
            &address,
            &value
        );

    if (parsed != 2)
    {
        printf(
            "error: usage wmem <addr> <value>\n"
        );
        return;
    }

    *((volatile uint32_t*)address) =
        value;

    printf(
        "write 0x%08X to 0x%08X\n",
        value,
        address
    );
}

extern api_t device_api[];

void help_callback(const char* args)
{
    for (
        int i = 0;
        device_api[i].command_name != NULL;
        i++
    )
    {
        printf(
            "Команда '%s': '%s'\n",
            device_api[i].command_name,
            device_api[i].command_help
        );
    }
}

/* ===== API TABLE ===== */

api_t device_api[] =
{
    {
        "version",
        version_callback,
        "get device version"
    },

    {
        "on",
        led_on_callback,
        "switch on led"
    },

    {
        "off",
        led_off_callback,
        "switch off led"
    },

    {
        "blink",
        led_blink_callback,
        "blink led"
    },

    {
        "set_period",
        led_blink_set_period_ms_callback,
        "set blink period ms"
    },

    {
        "mem",
        mem_callback,
        "read memory by address"
    },

    {
        "wmem",
        wmem_callback,
        "write value to address"
    },

    {
        "help",
        help_callback,
        "print commands description"
    },

    {
        NULL,
        NULL,
        NULL
    }
};

/* ===== MAIN ===== */

int main()
{
    stdio_init_all();

    stdio_task_init();

    protocol_task_init(
        device_api
    );

    led_task_init();

    while (true)
    {
        char* command =
            stdio_task_handle();

        if (command != NULL)
        {
            protocol_task_handle(
                command
            );
        }

        led_task_handle();
    }
}