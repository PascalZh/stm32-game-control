#ifndef __BREAKOUT_H__
#define __BREAKOUT_H__

#include "mbed.h"

// 定义游戏界面的尺寸和元素信息

// 定义游戏界面的尺寸和元素信息
#define BALL_RADIUS 2
#define BRICK_WIDTH 8
#define BRICK_HEIGHT 4
#define BRICK_NUM 6
#define PADDLE_WIDTH 32
#define PADDLE_HEIGHT 4
#define PADDLE_SPEED 2
#define BALL_SPEED 2

void breakout_game_loop(unsigned frame_delay_ms);

#endif /* __BREAKOUT_H__ */