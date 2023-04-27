#ifndef __SNAKE_H__
#define __SNAKE_H__

#define SNAKE_MAX_LENGTH 20
#define SNAKE_INITIAL_LENGTH 3
#define SNAKE_PIXEL_SIZE 4
#define SNAKE_PIXEL_MARGIN 1
#define SNAKE_WIDTH 80
#define SNAKE_HEIGHT 48

#define FOOD_PIXEL_SIZE 4
#define FOOD_PIXEL_MARGIN 1

typedef struct {
    int x;
    int y;
} point_t;

typedef enum {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} direction_t;

typedef enum {
    WHITE,
    RED,
    BLACK
} color_t;

void snake_init(void);
void snake_move(void);
void snake_draw(void);
void snake_clear(void);
int snake_get_score(void);
void snake_set_direction(direction_t direction);

void snake_game_loop(unsigned delay);

#endif /* __SNAKE_H__ */