#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

/* Макросы устройства и версии */
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

/* Глобальная переменная */
uint32_t global_variable = 0;

/* Глобальная константа */
const uint32_t constant_variable = 42;

int main()
{
    /* Инициализация stdio (USB CDC) */
    stdio_init_all();

    /* Небольшая задержка, чтобы терминал успел подключиться */
    sleep_ms(2000);

    while (true)
    {
        /* 1. Простая строка */
        printf("Hello World!\n");

        /* 2. Строка с аргументами */
        printf("device name: '%s', firmware version: %s\n",
               DEVICE_NAME,
               DEVICE_VRSN);

        /* 3. Таймштамп */
        uint64_t timestamp = time_us_64();
        printf("system timestamp: %llu us\n", timestamp);

        /* 4. Переменная на стеке */
        uint32_t stack_variable = 8888;

        printf("stack variable | addr = 0x%X | value = %u\n",
               &stack_variable,
               *(&stack_variable));

        printf("stack variable | addr = 0x%X | value = %X\n",
               &stack_variable,
               *(&stack_variable));

        printf("stack variable | addr = 0x%X | value = 0x%X\n",
               &stack_variable,
               *(&stack_variable));

        /* 5. Глобальная переменная */
        global_variable++;

        printf("global variable | addr = 0x%X | value = %u\n",
               &global_variable,
               *(&global_variable));

        /* 6. Переменная на куче (утечка памяти намеренно) */
        uint32_t* heap_variable =
            (uint32_t*)malloc(sizeof(uint32_t));

        *heap_variable = 5555;

        printf("heap variable | addr = 0x%X | value = %u\n",
               heap_variable,
               *heap_variable);

        /* ВАЖНО:
           free НЕ вызывается специально
           чтобы увидеть рост адресов */

        /* 7. Константа */
        printf("constant variable | addr = 0x%X | value = %u\n",
               &constant_variable,
               *(&constant_variable));

        /* 8. Строковая константа */
        printf(
            "constant string | addr = 0x%X | value = 0x%X, [%s]\n",
            DEVICE_NAME,
            *((uint32_t*)DEVICE_NAME),
            DEVICE_NAME
        );

        /* 9. Регистр RP2040 по адресу */
        printf(
            "reg chip id | addr = 0x%X | value = 0x%X\n",
            0x40000000,
            *((uint32_t*)0x40000000)
        );

        /* 10. Переменная по адресу */
        printf(
            "var by addr | addr = 0x%X | value = %u\n",
            &global_variable,
            *((uint32_t*)&global_variable)
        );

        /* 11. Адрес функции main */
        printf(
            "main function | addr = 0x%X | value = 0x%X\n",
            main,
            *((uint32_t*)main)
        );

        /* Задержка */
        sleep_ms(1000);
    }
}