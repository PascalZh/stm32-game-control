#include "snake.h"

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define SNAKE_WIDTH (WIDTH / SNAKE_PIXEL_SIZE)
#define SNAKE_HEIGHT (HEIGHT / SNAKE_PIXEL_SIZE)
#define SNAKE_MAX_LENGTH (SNAKE_WIDTH * SNAKE_HEIGHT)

typedef struct {
  int x;
  int y;
} point_t;

typedef enum { DIR_UP = 0, DIR_RIGHT, DIR_DOWN, DIR_LEFT } direction_t;

static point_t snake[SNAKE_MAX_LENGTH];
static int snake_length = SNAKE_INITIAL_LENGTH;
static direction_t snake_direction = DIR_RIGHT;
static int score = 0;

static point_t food = {0, 0};

static void generate_food(void) {
  food.x = rand() % SNAKE_WIDTH;
  food.y = rand() % SNAKE_HEIGHT;
}

static direction_t get_direction(void) {
  if (joystick.get_direction() == N && snake_direction != DIR_DOWN) {
    return DIR_UP;
  } else if (joystick.get_direction() == W && snake_direction != DIR_RIGHT) {
    return DIR_LEFT;
  } else if (joystick.get_direction() == E && snake_direction != DIR_LEFT) {
    return DIR_RIGHT;
  } else if (joystick.get_direction() == S && snake_direction != DIR_UP) {
    return DIR_DOWN;
  } else {
    return snake_direction;
  }
}

static void draw_pixel(int x, int y) { lcd.setPixel(x, y, true); }

static void draw_rect(int x, int y, int width, int height) {
  int i, j;

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      draw_pixel(x + i, y + j);
    }
  }
}

void snake_init(void) {
  int i;

  for (i = 0; i < SNAKE_MAX_LENGTH; i++) {
    snake[i].x = -1;
    snake[i].y = -1;
  }

  for (i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
    snake[i].x = SNAKE_WIDTH / 2 - i;
    snake[i].y = SNAKE_HEIGHT / 2;
  }

  snake_length = SNAKE_INITIAL_LENGTH;

  snake_direction = DIR_RIGHT;

  score = 0;

  generate_food();
}

static void snake_move(void) {
  int i;

  for (i = snake_length - 1; i > 0; i--) {
    snake[i].x = snake[i - 1].x;
    snake[i].y = snake[i - 1].y;
  }

  switch (snake_direction) {
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

  if (snake[0].x == food.x && snake[0].y == food.y) {
    score++;

    if (snake_length < SNAKE_MAX_LENGTH) {
      snake_length++;
    }

    generate_food();
  }
}

static void snake_draw(void) {
  int i;
  lcd.clear();

  for (i = 0; i < snake_length; i++) {
    draw_rect(snake[i].x * (SNAKE_PIXEL_SIZE), snake[i].y * (SNAKE_PIXEL_SIZE),
              SNAKE_PIXEL_SIZE, SNAKE_PIXEL_SIZE);
  }

  draw_rect(
      food.x * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN * 2) + FOOD_PIXEL_MARGIN,
      food.y * (FOOD_PIXEL_SIZE + FOOD_PIXEL_MARGIN * 2) + FOOD_PIXEL_MARGIN,
      FOOD_PIXEL_SIZE, FOOD_PIXEL_SIZE);
    lcd.refresh();
}

static bool is_game_over(void) {
  int i;

  if (snake[0].x < 0 || snake[0].x >= SNAKE_WIDTH || snake[0].y < 0 ||
      snake[0].y >= SNAKE_HEIGHT) {
    return true;
  }

  for (i = 1; i < snake_length; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      return true;
    }
  }

  return false;
}

int snake_get_score(void) { return score; }

static void snake_set_direction(direction_t direction) {
  snake_direction = direction;
}

void snake_game_loop(unsigned delay) {
  int i;
  direction_t direction;

  srand(time(NULL));

  snake_init();

  while (1) {

    for (int i = 0; i < 1000 / delay; i++) {
      direction = get_direction();
      snake_set_direction(direction);
      thread_sleep_for(delay);
    }

    snake_move();

    snake_draw();

    if (is_game_over()) {
      break;
    }
  }

  lcd.clear();

  static char buf[50];
  lcd.printString("Game over!", 13, 2);
  lcd.printString("Your score is:", 0, 3);
  sprintf(buf, "%d", snake_get_score());
  lcd.printString(buf, 42, 4);
  lcd.refresh();

  while (joystick.get_direction() == CENTRE) {
    thread_sleep_for(delay);
  }
}