#include "Snake.h"

AbRect segmentShape = {abRectGetBounds, abRectCheck, {2,2}}; // shape of snake segments

Layer tailLayer = {
    (AbShape *)&segmentShape,
    {(screenWidth/2),(screenHeight/2)+5},
    {0,0},{0,0},
    COLOR_BLUE,
    0
};

Layer headLayer = {
    (AbShape *)&segmentShape,
    {screenWidth/2, screenHeight/2},
    {screenWidth/2,screenHeight/2},{screenWidth/2,screenWidth/2},
    COLOR_WHITE,
    &tailLayer
};

Vec2 direction = (Vec2){0,-1};

Vec2 segments[25];

Snake s = {
    &headLayer,
    &tailLayer,
    &direction,
    0, // size
    segments
};

Snake *snake = &s;

void snakeInit() {
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
    for (i = 24; i > 0; i--) { // update segment position list
        snake->segments[i] = snake->segments[i - 1];
    }
    
    snake->segments[0] = snake->headLayer->posLast; // update first non-head segment
    snake->tailLayer->posLast = snake->tailLayer->pos;
    snake->tailLayer->pos = snake->segments[snake->size];
}

// checks if the snake has collided with itself, returning true if so
bool snakeIsEatingSelf() {
    return false;
}

// checks if the snake has collided with an apple, returning true if so
bool snakeIsEatingApple() {
    return false;
}

// checks if the snake has collided with the bounds, returning true if so
bool snakeIsOutOfBounds(Snake* snake, Region* bounds) {
    if (snake != 0 && bounds != 0) {
        Layer* headLayer = snake->headLayer; // only need to check the head (it's snake yo)
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

// draws a snake
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
            u_int color = COLOR_BLUE;
            if (abShapeCheck(apple->appleLayer->abShape, &apple->appleLayer->pos, &pixelPos)) {
                color = apple->appleLayer->color;
            }
            lcd_writeColor(color);
        } // for col
    } // for row
}
