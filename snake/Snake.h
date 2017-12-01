#ifndef SNAKE__H_
#define SNAKE__H_

#include <shape.h>
#include <stdbool.h>
#include <stdlib.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "Apple.h"

typedef struct Snake {
  Layer* headLayer;
  Layer* tailLayer;
  Vec2* direction;
} Snake;

Snake* snakeInit();

void snakeUpdate(Snake* snake);

bool snakeIsEatingSelf(Snake* snake);

bool snakeIsEatingApple(Snake* snake, Apple* apple);

bool snakeIsOutOfBounds(Snake* snake, Region* bounds);

void snakeGrow(Snake* snake);

void snakeDraw(Snake* snake);

#endif
