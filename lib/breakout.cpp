#include "breakout.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#define SCREEN_WIDTH WIDTH
#define SCREEN_HEIGHT (HEIGHT - 8)
#define CANVAS_Y0 8

// 定义游戏元素的结构体
typedef struct {
  int x;
  int y;
  int w;
  int h;
} Rect;

typedef struct {
  int x;
  int y;
  int r;
  int vx;
  int vy;
} Circle;

// 定义游戏状态的枚举类型
typedef enum { GAME_START, GAME_RUNNING, GAME_OVER } GameState;
typedef enum { MOVE_LEFT, MOVE_RIGHT, MOVE_NONE } Movement;

// 初始化游戏界面和元素
void init_game(Rect *bricks, Rect *paddle, Circle *ball) {
  // 初始化砖块
  for (int i = 0; i < BRICK_NUM; i++) {
    bricks[i].x = (i % 8) * BRICK_WIDTH;
    bricks[i].y = (i / 8) * BRICK_HEIGHT;
    bricks[i].w = BRICK_WIDTH;
    bricks[i].h = BRICK_HEIGHT;
  }
  // 初始化挡板
  paddle->x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
  paddle->y = SCREEN_HEIGHT - PADDLE_HEIGHT;
  paddle->w = PADDLE_WIDTH;
  paddle->h = PADDLE_HEIGHT;
  // 初始化球
  ball->x = SCREEN_WIDTH / 2;
  ball->y = SCREEN_HEIGHT / 2;
  ball->r = BALL_RADIUS;
  ball->vx = BALL_SPEED;
  ball->vy = BALL_SPEED;
}

bool is_intersect(Circle *ball, Rect *rect) {
  // 矩形的中心点
  int cx = rect->x + rect->w / 2;
  int cy = rect->y + rect->h / 2;
  // 小球与矩形中心点的距离
  int dx = abs(ball->x - cx);
  int dy = abs(ball->y - cy);
  // 小球与矩形中心点的距离小于矩形的宽度和高度的一半，说明小球与矩形相交
  if (dx <= rect->w / 2 + ball->r && dy <= rect->h / 2 + ball->r) {
    return true;
  }
  return false;
}

Movement get_movement() {
  if (joystick.get_direction() == E) {
    return MOVE_RIGHT;
  } else if (joystick.get_direction() == W) {
    return MOVE_LEFT;
  } else {
    return MOVE_NONE;
  }
}

void update_paddle(Rect *paddle, Movement movement) {
  if (movement == MOVE_LEFT) {
    paddle->x -= PADDLE_SPEED;
  } else if (movement == MOVE_RIGHT) {
    paddle->x += PADDLE_SPEED;
  }
  if (paddle->x < 0) {
    paddle->x = 0;
  } else if (paddle->x + paddle->w > SCREEN_WIDTH) {
    paddle->x = SCREEN_WIDTH - paddle->w;
  }
}

// 更新球的位置
void update_ball(Circle *ball, Rect *paddle, Rect *bricks, int *score,
                 GameState *state) {
  static int last_paddle_x = 0;
  // 根据小球的速度和方向，计算小球的新位置
  ball->x += ball->vx;
  ball->y += ball->vy;

  // 检测小球是否撞到边界
  if (ball->x - ball->r <= 0 || ball->x + ball->r >= SCREEN_WIDTH) {
    ball->vx = -ball->vx;
  }
  if (ball->y - ball->r <= 0) {
    ball->vy = -ball->vy;
  }
  if (ball->y + ball->r >= SCREEN_HEIGHT) {
    *state = GAME_OVER;
  }

  // 检测小球是否撞到挡板
  if (is_intersect(ball, paddle)) {
    ball->vy = -ball->vy;
    // 根据小球与挡板的相对速度和摩擦
    int v_paddle = paddle->x - last_paddle_x;
    if (v_paddle > 0) {
      ball->vx += 1;
    } else if (v_paddle < 0) {
      ball->vx -= 1;
    }
    if (ball->vx > 3) {
      ball->vx = 3;
    }
    if (ball->vx < -3) {
      ball->vx = -3;
    }
  }

  // 检测小球是否撞到砖块
  for (int i = 0; i < BRICK_NUM; i++) {
    if (bricks[i].w > 0 && is_intersect(ball, &bricks[i])) {
      bricks[i].w = 0;
      bricks[i].h = 0;
      *score += 1;

      int dx = ball->x - (bricks[i].x + bricks[i].w / 2);
      int dy = ball->y - (bricks[i].y + bricks[i].h / 2);
      if (abs(dx) > abs(dy) * BRICK_WIDTH / BRICK_HEIGHT && dx * ball->vx < 0) {  // dx * ball->vx < 0 is important!
        ball->vx = -ball->vx;
      } else {
        ball->vy = -ball->vy;
      }
      // 如果所有砖块都被打掉了，游戏胜利
      bool win = true;
      for (int j = 0; j < BRICK_NUM; j++) {
        if (bricks[j].w > 0) {
          win = false;
          break;
        }
      }
      if (win) {
        *state = GAME_OVER;
      }
      break;
    }
  }

  last_paddle_x = paddle->x;
}

// 绘制游戏界面
void draw_game(Rect *bricks, Rect *paddle, Circle *ball, int score) {
  lcd.clear();
  // 绘制砖块
  for (int i = 0; i < BRICK_NUM; i++) {
    if (bricks[i].w > 0) {
      lcd.drawRect(bricks[i].x + 1, bricks[i].y + 1 + CANVAS_Y0,
                   bricks[i].w - 1, bricks[i].h - 1, FILL_BLACK);
    }
  }

  // 绘制挡板
  lcd.drawRect(paddle->x, paddle->y + CANVAS_Y0, paddle->w, paddle->h,
               FILL_BLACK);

  // 绘制小球
  lcd.drawCircle(ball->x, ball->y + CANVAS_Y0, ball->r, FILL_BLACK);

  // 绘制分数
  char score_text[20];
  sprintf(score_text, "Score: %d", score);
  lcd.printString(score_text, 0, 0);

  lcd.refresh();
}

void breakout_game_loop(unsigned delay) {
  Rect bricks[BRICK_NUM];
  Rect paddle;
  Circle ball;
  GameState state = GAME_START;
  int score = 0;

  init_game(bricks, &paddle, &ball);

  while (state != GAME_OVER) {
    if (state == GAME_START) {
      lcd.clear();
      lcd.printString("Move joystick", 0, 0);
      lcd.printString("to start...", 0, 1);
      lcd.refresh();

      // 等待用户按下按键
      while (joystick.get_direction() == CENTRE) {
        thread_sleep_for(10);
      }
      state = GAME_RUNNING;
    }

    // 处理输入
    Movement move = get_movement();
    update_paddle(&paddle, move);

    // 更新游戏状态
    update_ball(&ball, &paddle, bricks, &score, &state);

    // 绘制游戏界面
    draw_game(bricks, &paddle, &ball, score);

    // 延时
    thread_sleep_for(delay);

    if (state == GAME_OVER) {
      break;
    }
  }

  lcd.clear();
  lcd.printString("Game Over!", 13, 3);
  lcd.refresh();
  thread_sleep_for(500);
}
