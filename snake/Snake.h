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
    Vec2 segments[25];
} Snake;

extern Snake *snake;

void snakeInit();

void snakeUpdate();

bool snakeIsEatingSelf();

bool snakeIsEatingApple();

bool snakeIsOutOfBounds(Region* bounds);

void snakeGrow();

void snakeDraw();

#endif
