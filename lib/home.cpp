#include "home.h"
#include "main.h"

#include "snake.h"
#include "breakout.h"
#include <stdio.h>

static void draw_home();
static void draw_options(char** options, int selected_option);
static void options_loop(unsigned delay);

static void draw_home()
{
    static const char *ascii_art[] = {
R"( ___  ___  _  _ )",
R"((_  )/ __)( \/ ))",
R"( / / \__ \ \  / )",
R"((___)(___/(__/  )",
"Move joystick",
"to continue..."
};
    lcd.clear();
    for (int i = 0; i < 6; i++)
    {
        lcd.printString(ascii_art[i], 0, i);
    }
    lcd.refresh();
}

void home_loop(unsigned delay)
{
    // Show the home screen

    // 1. show an ascii art picture
    draw_home();
    // 2. wait for user input
    while (true)
    {
        if (joystick.get_direction() != CENTRE)
        {
            thread_sleep_for(delay * 2);  // delay consecutive joystick.get_direction() call in options_loop
            options_loop(delay);

            // when options_loop exit, show the home screen again
            draw_home();
        }
        thread_sleep_for(delay);
    }
}

static void draw_options(char** options, int num_options, int selected_option)
{
    lcd.clear();
    const unsigned indent_width = 10;

    for (int i = 0; i < BANKS; i++)
    {
        if (i < num_options) {
            lcd.printString(options[i], indent_width, i);
            if (i == selected_option)
            {
                lcd.printString(">", 0, i);
                lcd.printString("<", WIDTH-5, i);
            }
        }
    }
    lcd.refresh();
}


static void options_loop(unsigned delay)
{
    // Show the options screen
    static char* options[] = {
        "1. Snake",
        "2. Breakout",
        "3. Settings",
        "4. Quit"
    };
    int selected_option = 0;

    while (true)
    {
        // 1. show the options
        draw_options(options, 4, selected_option);

        // 2. wait for user input
        if (joystick.get_direction() == N) {
            thread_sleep_for(delay);
            selected_option = (selected_option - 1 + 4) % 4;
        } else if (joystick.get_direction() == S) {
            thread_sleep_for(delay);
            selected_option = (selected_option + 1) % 4;
        } else if (joystick.get_direction() == E) {
            thread_sleep_for(delay * 2);
            switch (selected_option)
            {
                case 0:
                    snake_game_loop(50);
                    break;
                case 1:
                    breakout_game_loop(delay);
                    break;
                case 2:
                    break;
                case 3:
                    return;
            }
        }

        thread_sleep_for(delay);
    }
}
