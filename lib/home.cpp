#include "home.h"
#include "main.h"
#include <stdio.h>

static void draw_home();
static void draw_options(char** options, int selected_option);
static void options_loop(unsigned delay);

static void draw_home()
{
    static const char *ascii_art = 
R"ZSY(
 ___  ___  _  _ 
(_  )/ __)( \/ )
 / / \__ \ \  / 
(___)(___/(__/  
                
Press button1...
)ZSY";
    for (int i = 0; i < 6; i++)
    {
        lcd.printString(ascii_art + i * 16 + 1, 0, i);
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
        if (button1 == 0)
        {
            lcd.clear();
            lcd.refresh();

            options_loop(delay);

            // when options_loop exit, show the home screen again
            draw_home();
        }
        thread_sleep_for(delay);
    }
}

static void draw_options(char** options, int selected_option)
{
    const unsigned indent_width = 10;

    static int option_count = sizeof(options) / sizeof(options[0]);

    for (int i = 0; i < BANKS; i++)
    {
        if (selected_option < BANKS) {
            lcd.printString(options[i], indent_width, i);
        } else if (option_count - selected_option < BANKS) {
            lcd.printString(options[option_count - BANKS + i], indent_width, i);
        } else {
            lcd.printString(options[selected_option - BANKS / 2 + i], indent_width, i);
        }

        if (i == BANKS / 2)
        {
            lcd.printString(">", 0, i);
            lcd.printString("<", WIDTH-5, i);
        }
    }
}


static void options_loop(unsigned delay)
{
    // Show the options screen
    static char* options[] = {
        "1. Snake",
        "2. Tetris",
        "3. Settings",
        "4. Quit"
    };
    int selected_option = 0;

    while (true)
    {
        // 1. show the options
        draw_options(options, selected_option);

        // 2. wait for user input
        thread_sleep_for(delay);
    }
}
