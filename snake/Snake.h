#ifndef SNAKE__H_
#define SNAKE__H_

#include <shape.h>
#include <stdbool.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "Apple.h"

// snake struct
typedef struct Snake {
    Layer* headLayer; // layer of the head
    Layer* tailLayer; // layer of the tail
    Vec2* direction; // snake's direction
    char size; // size of the snake (0 = just head)
    Vec2 segments[25]; // position history
} Snake;

// external reference to the internal snake struct
extern Snake *snake;

// resets snake position, size, and initializes segments
void snakeInit(void);

// updates all the snake segments according to direction vector
void snakeUpdate(void);

// changes direction and snake's next position
// param dir :: a bit vector indicating what direction to change to
void snakeChangeDirection(char dir);

// checks if the snake has collided with itself, returning true if so
bool snakeIsEatingSelf(void);

// checks if the snake has collided with an apple, returning true if so
bool snakeIsEatingApple(void);

// checks if the snake has collided with the bounds, returning true if so
bool snakeIsOutOfBounds(Region* bounds);

// adds one segment to the snake
void snakeGrow(void);

// draws the snake
void snakeDraw(void);

#endif
