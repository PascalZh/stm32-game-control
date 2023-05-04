#ifndef __SNAKE_H__
#define __SNAKE_H__

#define SNAKE_PIXEL_SIZE 4
#define SNAKE_INITIAL_LENGTH 3
#define FOOD_PIXEL_SIZE 2
#define FOOD_PIXEL_MARGIN 1


int snake_get_score(void);

void snake_game_loop(unsigned delay);

#endif /* __SNAKE_H__ */