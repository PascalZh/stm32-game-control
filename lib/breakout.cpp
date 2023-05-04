#include "breakout.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#define SCREEN_WIDTH WIDTH
#define SCREEN_HEIGHT (HEIGHT - 7)
#define CANVAS_Y0 7

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
    bricks[i].x = i * BRICK_WIDTH;
    bricks[i].y = 0;
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

// 检测两个矩形是否相交
bool is_intersect(Rect *rect1, Rect *rect2) {
  return (rect1->x < rect2->x + rect2->w && rect1->x + rect1->w > rect2->x &&
          rect1->y < rect2->y + rect2->h && rect1->y + rect1->h > rect2->y);
}

Movement get_movement() {
  if (button1 == 0) {
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
  if (is_intersect((Rect *)ball, paddle)) {
    ball->vy = -ball->vy;
    // 根据小球与挡板的相对位置，调整小球的反弹方向和速度
    int dist = ball->x - (paddle->x + paddle->w / 2);
    ball->vx = dist * 0.2;
  }

  // 检测小球是否撞到砖块
  for (int i = 0; i < BRICK_NUM; i++) {
    if (bricks[i].w > 0 && is_intersect((Rect *)ball, &bricks[i])) {
      bricks[i].w = 0;
      bricks[i].h = 0;
      *score += 1;
      ball->vy = -ball->vy;
      // 根据小球与砖块的相对位置，调整小球的反弹方向和速度
      int dist = ball->x - (bricks[i].x + bricks[i].w / 2);
      ball->vx = dist * 0.2;
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
    }
  }
}

// 绘制游戏界面
void draw_game(Rect *bricks, Rect *paddle, Circle *ball, int score) {
  // 绘制砖块
  for (int i = 0; i < BRICK_NUM; i++) {
    if (bricks[i].w > 0) {
      lcd.drawRect(bricks[i].x, bricks[i].y + CANVAS_Y0, bricks[i].w,
                   bricks[i].h, FILL_BLACK);
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

}

void breakout_game_loop(unsigned frame_delay_ms) {
  Rect bricks[BRICK_NUM];
  Rect paddle;
  Circle ball;
  GameState state = GAME_START;
  int score = 0;

  init_game(bricks, &paddle, &ball);

  while (state != GAME_OVER) {
    lcd.clear();
    if (state == GAME_START) {
      lcd.printString("Press any key to start", 0, 0);
      lcd.refresh();

      // 等待用户按下按键
      while (button1 == 1){
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

    lcd.refresh();

    // 延时
    thread_sleep_for(frame_delay_ms);

  }

  lcd.clear();
  lcd.printString("Game Over", 0, 0);
  lcd.refresh();
  thread_sleep_for(1000);
}
