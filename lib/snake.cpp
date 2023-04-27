#include "snake.h"

static point_t snake[SNAKE_MAX_LENGTH];
static int snake_length = SNAKE_INITIAL_LENGTH;
static direction_t snake_direction = DIR_NONE;
static int score = 0;

static point_t food = {0, 0};

static void generate_food(void)
{
    // food.x = rand() % SNAKE_WIDTH;
    // food.y = rand() % SNAKE_HEIGHT;
}

static direction_t get_direction(void)
{
    //! TODO
    return DIR_NONE;
}

static void draw_pixel(int x, int y, color_t color)
{
    int i, j;

    for (i = 0; i < SNAKE_PIXEL_SIZE; i++)
    {
        for (j = 0; j < SNAKE_PIXEL_SIZE; j++)
        {
            // lcd_draw_pixel(x + i, y + j, color);
        }
    }
}

static void draw_rect(int x, int y, int width, int height, color_t color)
{
    int i, j;

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            draw_pixel(x + i, y + j, color);
        }
    }
}


void snake_init(void)
{
    int i;

    for (i = 0; i < SNAKE_MAX_LENGTH; i++)
    {
        snake[i].x = 0;
        snake[i].y = 0;
    }

    snake[0].x = SNAKE_WIDTH / 2;
    snake[0].y = SNAKE_HEIGHT / 2;

    snake_direction = DIR_NONE;

    score = 0;

    generate_food();
}

void snake_move(void)
{
    int i;

    if (snake_direction == DIR_NONE)
    {
        return;
    }

    for (i = snake_length - 1; i > 0; i--)
    {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    switch (snake_direction)
    {
    case DIR_UP:
        snake[0].y--;
        break;
    case DIR_DOWN:
        snake[0].y++;
        break;
    case DIR_LEFT:
        snake[0].x--;
        break;
    case DIR_RIGHT:
        snake[0].x++;
        break;
    default:
        break;
    }

    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        score++;

        if (snake_length < SNAKE_MAX_LENGTH)
        {
            snake_length++;
        }

        generate_food();
    }
}

void snake_draw(void)
{
    int i;

    for (i = 0; i < snake_length; i++)
    {
        draw_rect(snake[i].x * (SNAKE_PIXEL_SIZE + SNAKE_PIXEL_MARGIN),
                  snake[i].y * (SNAKE_PIXEL_SIZE + SNAKE_PIXEL_MARGIN),
                  SNAKE_PIXEL_SIZE, SNAKE_PIXEL_SIZE, WHITE);
    }

    draw_rect(food.x * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN),
              food.y * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN),
              FOOD_PIXEL_SIZE, FOOD_PIXEL_SIZE, RED);
}

void snake_clear(void)
{
    int i;

    for (i = 0; i < snake_length; i++)
    {
        draw_rect(snake[i].x * (SNAKE_PIXEL_SIZE + SNAKE_PIXEL_MARGIN),
                  snake[i].y * (SNAKE_PIXEL_SIZE + SNAKE_PIXEL_MARGIN),
                  SNAKE_PIXEL_SIZE, SNAKE_PIXEL_SIZE, BLACK);
    }

    draw_rect(food.x * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN),
              food.y * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN),
              FOOD_PIXEL_SIZE, FOOD_PIXEL_SIZE, BLACK);
}

int snake_get_score(void)
{
    return score;
}

void snake_set_direction(direction_t direction)
{
    snake_direction = direction;
}

void snake_game_loop(unsigned delay)
{
    // int i;
    // direction_t direction;

    // srand(time(NULL));

    // init_graphics();

    // snake_init();

    // while (1)
    // {
    //     clear_screen();

    //     direction = get_direction();

    //     snake_set_direction(direction);

    //     snake_move();

    //     snake_draw();

    //     update_screen();

    //     HAL_Delay(delay);

    //     if (is_game_over())
    //     {
    //         break;
    //     }
    // }

    // clear_screen();

    // printf("Game over! Your score is: %d\n", snake_get_score());

    // close_graphics();
}