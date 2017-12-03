#ifndef SNAKE__H_
#define SNAKE__H_

#include <shape.h>
#include <stdbool.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "Apple.h"

typedef struct Snake {
    Layer* headLayer;
    Layer* tailLayer;
    Vec2* direction;
    char size;
    Vec2[5] segments;
} Snake;

void snakeUpdate();

bool snakeIsEatingSelf();

bool snakeIsEatingApple();

bool snakeIsOutOfBounds(Snake* snake, Region* bounds);

void snakeGrow();

void snakeDraw();

#endif
