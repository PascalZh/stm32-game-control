/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "main.h"
#include "snake.h"
#include "home.h"

#define WAIT_TIME_MS 500 
DigitalOut led1(LED1);
DigitalIn button1(BUTTON1);
N5110 lcd(ARDUINO_UNO_D8, ARDUINO_UNO_D4, ARDUINO_UNO_D7, ARDUINO_UNO_D11, ARDUINO_UNO_D13, ARDUINO_UNO_D6);
Joystick joystick(PC_3, PC_2);

int main()
{
    lcd.init(LPH7366_6);
    lcd.setContrast(0.6);
    lcd.printString("Hello world!", 0, 0);
    lcd.refresh();
    joystick.init();

    thread_sleep_for(500);
    joystick.init();

    home_loop(100);
    printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    while (true)
    {
    
        Vector2D coord = joystick.get_coord();
        printf("Coord = %f,%f\n", coord.x, coord.y);

        float mag = joystick.get_mag();
        float angle = joystick.get_angle();
        printf("Mag = %f Angle = %f\n", mag, angle);
        
        Direction d = joystick.get_direction();
        printf("Direction = %i\n",d);
        thread_sleep_for(WAIT_TIME_MS);
    }
}
