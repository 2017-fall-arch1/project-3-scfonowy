#include "Snake.h"

AbRect headShape = {abRectGetBounds, abRectCheck, {2,2}}; // shape of snake segments

// creates and returns a new snake object of size 1
Snake* snakeInit() {
  Snake* snake = (Snake *)malloc(sizeof(Snake)); // create snake object
  
  Layer* headLayer = (Layer *)malloc(sizeof(Layer)); // create & configure head shape layer
  headLayer->abShape = (AbRect *)&headShape;
  headLayer->pos = (Vec2){(screenWidth/2), (screenHeight/2)};
  headLayer->posLast = (Vec2){0,0};
  headLayer->posNext = (Vec2){0,0};
  headLayer->color = COLOR_WHITE;
  headLayer->next = 0;
  
  Vec2* direction = (Vec2 *)malloc(sizeof(Vec2)); // create & configure direction vector
  direction->axes[0] = 0; // x = 1 for left, -1 for right, 0 for no direction
  direction->axes[1] = 1; // y = 1 for up, -1 for down, 0 for no direction
  
  snake->headLayer = headLayer; // configure snake object
  snake->tailLayer = 0;
  snake->direction = direction;
  
  return snake;
}


// updates all the snake segments according to direction vector
void snakeUpdate(Snake* snake) {
  if (snake != 0) {
    snake->headLayer->posLast = snake->headLayer->pos;
    snake->headLayer->pos = snake->headLayer->posNext;
    vec2add(snake->headLayer->posNext, snake->headLayer->posNext, snake->headLayer->direction);
    
    Layer* bodyLayerOne = snake->headLayer;
    Layer* bodyLayerTwo = snake->headLayer->next;
    
    while (bodyLayerOne != 0 && bodyLayerTwo != 0) { // iterate through and update positions of body segments
      bodyLayerTwo->posNext = bodyLayerOne->pos;
      bodyLayerTwo->posLast = bodyLayerTwo->pos;
      bodyLayerTwo->pos = bodyLayerOne->posLast;
      bodyLayerOne = bodyLayerOne->next;
      bodyLayerTwo = bodyLayerTwo->next;
    }
  }
}

// checks if the snake has collided with itself, returning true if so
bool snakeIsEatingSelf(Snake* snake) {
  if (snake != 0) {
    //TODO:- Check collision
  }
  
  return false; // a nonexistant snake cannot eat itself
}

// checks if the snake has collided with an apple, returning true if so
bool snakeIsEatingApple(Snake* snake, Apple* apple) {
  if (snake != 0 && apple != 0) {
    Region snakeBounds;
    Region appleBounds;
    abShapeGetBounds(apple->appleLayer->abShape, apple->appleLayer->pos, &appleBounds);
    abShapeGetBounds(snake->headLayer->abShape, snake->headLayer->pos, &snakeBounds);
    int row;
    int col;
    for (row = appleBounds.topLeft.axes[1]; row <= appleBounds.botRight.axes[1]; row++) {
      for (col = appleBounds.topLeft.axes[0]; col <= appleBounds.botRight.axes[0]; col++) {
        Vec2 pixelPos = (Vec2){row, col};
        if (abShapeCheck(snake->headLayer->abShape, snake->headLayer->pos, &pixelPos)) {
          return true;
        }
      }
    }
  }
  return false;
}

// checks if the snake has collided with the bounds, returning true if so
bool snakeIsOutOfBounds(Snake* snake, Region* bounds) {
  if (snake != 0 && bounds != 0) {
    Layer* headLayer = snake->headLayer; // only need to check the head (it's snake yo)
    Region snakeBounds;
    abShapeGetBounds(snake->headLayer->abShape, snake->headLayer->pos, &snakeBounds);
    int axis;
    for (axis = 0; axis < 2; axis++) {
      if ((snakeBounds.topLeft.axes[axis] < bounds->topLeft.axes[axis]) || (snakeBounds.botRight.axes[axis] > bounds->botRight.axes[axis]) ) {
        return true;
      }
    }
  }
  return false;
}

// adds one segment to the snake (is overlaid on top of the tail for initial cycle)
void snakeGrow(Snake* snake) {
  if (snake != 0) {
    Layer* newLayer = (Layer *)malloc(sizeof(Layer)); // create & configure the new segment layer
    newLayer->abShape = (AbRect *)&headShape;
    newLayer->posLast = (Vec2){0,0}; // will be set by snakeUpdate()
    newLayer->posNext = (Vec2){0,0};
    newLayer->color = COLOR_WHITE;
    newLayer->next = 0;
    
    if (snake->tailLayer != 0) { // default cause
      newLayer->pos = snake->tailLayer->pos;
      snake->tailLayer->next = newLayer;
    } else { // special case if this is the first additional segment
      newLayer->pos = snake->headLayer->pos;
      snake->headLayer->next = newLayer;
    }
    
    snake->tailLayer = newLayer;
  }
}

// draws a snake
void snakeDraw(Snake* snake) {
  int row, col;
  Layer* snakeLayer;
  
  for (snakeLayer = snake->headLayer; snakeLayer; snakeLayer = snakeLayer->next) { /* for each snake layer */
    Region bounds;
    layerGetBounds(snakeLayer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
                bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
        Vec2 pixelPos = (Vec2){col, row};
        u_int color = COLOR_BLUE;
        Layer* probeLayer;
        for (probeLayer = snake->headLayer; probeLayer; probeLayer = probeLayer->next) { /* probe all layers, in order */
          if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
            color = probeLayer->color;
            break; 
          } /* if probe check */
        } // for checking all layers at col, row
        lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}
