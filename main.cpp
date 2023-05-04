/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "main.h"
#include "snake.h"

#define WAIT_TIME_MS 500 
DigitalOut led1(LED1);
DigitalIn button1(BUTTON1);
N5110 lcd(ARDUINO_UNO_D8, ARDUINO_UNO_D4, ARDUINO_UNO_D7, ARDUINO_UNO_D11, ARDUINO_UNO_D13, ARDUINO_UNO_D6);

int main()
{
    lcd.init(LPH7366_1);
    lcd.setContrast(0.4);
    lcd.printString("Hello world!", 0, 0);
    lcd.refresh();

    thread_sleep_for(3000);

    snake_game_loop(1000);
    // printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    // while (true)
    // {
    //     led1 = !led1;
    //     thread_sleep_for(WAIT_TIME_MS);
    // }
}
