#ifndef __SNAKE_H__
#define __SNAKE_H__

#define SNAKE_PIXEL_SIZE 4
#define SNAKE_WIDTH (84 / SNAKE_PIXEL_SIZE)
#define SNAKE_HEIGHT (48 / SNAKE_PIXEL_SIZE)
#define SNAKE_MAX_LENGTH (SNAKE_WIDTH * SNAKE_HEIGHT)
#define SNAKE_INITIAL_LENGTH 3
#define FOOD_PIXEL_SIZE 2
#define FOOD_PIXEL_MARGIN 1

typedef struct {
    int x;
    int y;
} point_t;

typedef enum {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
} direction_t;

int snake_get_score(void);

void snake_game_loop(unsigned frame_delay_ms);

#endif /* __SNAKE_H__ */