#ifndef SNAKE__H_
#define SNAKE__H_

#include <shape.h>
#include <stdbool.h>
#include <stdlib.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "Apple.h"

typedef struct Snake {
  struct Layer* headLayer;
  struct Layer* tailLayer;
  struct Vec2* direction;
} Snake;

Snake* snakeInit();

void snakeUpdate(Snake* snake);

bool snakeIsEatingSelf(Snake* snake);

bool snakeIsEatingApple(Snake* snake, Apple* apple);

bool snakeIsOutOfBounds(Snake* snake, Region* bounds);

void snakeGrow(Snake* snake);

#endif
