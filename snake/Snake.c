#include "Snake.h"

// the shape of snake segments
AbRect segmentShape = {abRectGetBounds, abRectCheck, {2,2}};

// layer of the tail, same color as background
Layer tailLayer = {
  (AbShape *)&segmentShape,
  {(screenWidth/2),(screenHeight/2)+5},
  {0,0},{0,0},
  COLOR_BLACK,
  0
};

// layer of the head, white color
Layer headLayer = {
  (AbShape *)&segmentShape,
  {screenWidth/2, screenHeight/2},
  {screenWidth/2,screenHeight/2},{screenWidth/2,screenWidth/2},
  COLOR_WHITE,
  &tailLayer
};

// direction vector of the snake
// X: -1 is left, 1 is right
// Y: -1 is up, 1 is down
Vec2 direction = (Vec2){-1,0};

// history of the head's positions, i.e. the segments
Vec2 segments[25];

// actual snake struct
Snake s = {
  &headLayer,
  &tailLayer,
  &direction,
  0, // size
  segments
};

// external reference to the snake struct
Snake *snake = &s;

// resets snake position, size, and initializes segments
void snakeInit() {
  // reset snake position
  snake->headLayer->pos = (Vec2){screenWidth/2, screenHeight/2};
  snake->headLayer->posNext = (Vec2){screenWidth/2, screenHeight/2};
  snake->tailLayer->pos = (Vec2){screenWidth/2, (screenHeight/2)+5};
  
  // reset snake size
  snake->size = 0;
  
  int i;
  // initialize all the segments
  for (i = 0; i < 25; i++) {
    snake->segments[i] = snake->headLayer->pos;
  }
}

// updates all the snake segments according to direction vector
void snakeUpdate() {
  // save prior position
  snake->headLayer->posLast = snake->headLayer->pos;
  snake->headLayer->pos = snake->headLayer->posNext;
  
  // update position
  snake->headLayer->posNext.axes[0] += 5*snake->direction->axes[0];
  snake->headLayer->posNext.axes[1] += 5*snake->direction->axes[1];
  
  int i;
  // propogate previous positions down the list
  for (i = 24; i > 0; i--) {
    snake->segments[i] = snake->segments[i - 1];
  }
  
  snake->segments[0] = snake->headLayer->posLast; // update first non-head segment
  
  // update tail
  snake->tailLayer->posLast = snake->tailLayer->pos;
  snake->tailLayer->pos = snake->segments[snake->size];
}

// changes direction and snake's next position
// param dir :: a bit vector indicating what direction to change to
void snakeChangeDirection(char dir) {
  // we don't want to register input for the same axis the snake is already moving
  // since the snake is either already moving in that direction, or cannot reverse
  if (dir  == 1 && snake->direction->axes[0] == 0) { // left
    snake->direction->axes[0] = -1;
    snake->direction->axes[1] = 0;
  } else if (dir == 2 && snake->direction->axes[1] == 0) { // up
    snake->direction->axes[0] = 0;
    snake->direction->axes[1] = -1;
  } else if (dir == 4 && snake->direction->axes[1] == 0) { // down
    snake->direction->axes[0] = 0;
    snake->direction->axes[1] = 1;
  } else if (dir == 8 && snake->direction->axes[0] == 0) { // right
    snake->direction->axes[0] = 1;
    snake->direction->axes[1] = 0;
  }
  
  // update the posNext vector for the new direction
  snake->headLayer->posNext.axes[0] = snake->headLayer->pos.axes[0] + 5*snake->direction->axes[0];
  snake->headLayer->posNext.axes[1] = snake->headLayer->pos.axes[1] + 5*snake->direction->axes[1];
}

// checks if the snake has collided with itself, returning true if so
bool snakeIsEatingSelf() {
  // since we're moving along a "grid", we only need to check
  // if the snake's head shares a position with any of its history positions
  // up to the size of the snake
  int i;
  for (i = 0; i < size; i++) {
    if (snake->headLayer->pos.axes[0] == snake->segments[i].axes[0]
        && snake->headLayer->pos.axes[1] == snake->segments[i].axes[1]) {
      return true;
    }
  }
  return false;
}

// checks if the snake has collided with an apple, returning true if so
bool snakeIsEatingApple() {
  Region bounds;
  //layerGetBounds(snake->headLayer, &bounds);
  abShapeGetBounds(snake->headLayer->abShape, &snake->headLayer->pos, &bounds);
  int row;
  int col;
  for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
      Vec2 pixelPos = {col, row};
      if (abShapeCheck(apple->appleLayer->abShape, &apple->appleLayer->pos, &pixelPos)) {
        return true;
      }
    }
  }
  return false;
}

// checks if the snake has collided with the bounds, returning true if so
bool snakeIsOutOfBounds(Region* bounds) {
  if (bounds != 0) {
    // only need to check the head (it's snake yo)
    Region snakeBounds;
    abShapeGetBounds(snake->headLayer->abShape, &snake->headLayer->posNext, &snakeBounds);
    int axis;
    for (axis = 0; axis < 2; axis++) {
      if ((snakeBounds.topLeft.axes[axis] < bounds->topLeft.axes[axis]) || (snakeBounds.botRight.axes[axis] > bounds->botRight.axes[axis]) ) {
        return true;
      }
    }
  }
  return false;
}

// adds one segment to the snake
void snakeGrow() {
  if (snake->size < 24) {
    snake->size++;
  }
}

// draws the snake
void snakeDraw() {
  // since this is snake, we really only need to draw the head
  // and then clear the space left by the tail
  int row, col;
  Region bounds;
  
  // draw head
  layerGetBounds(snake->headLayer, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
  for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
      lcd_writeColor(snake->headLayer->color);
    }
  }
  
  // draw tail
  layerGetBounds(snake->tailLayer, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
  // apple may have spawned "under" the tail, so we want to draw it instead of blue
  for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
      Vec2 pixelPos = {col, row};
      u_int color = snake->tailLayer->color;
      if (abShapeCheck(apple->appleLayer->abShape, &apple->appleLayer->pos, &pixelPos)) {
        color = apple->appleLayer->color;
      }
      lcd_writeColor(color);
    } // for col
  } // for row
}
