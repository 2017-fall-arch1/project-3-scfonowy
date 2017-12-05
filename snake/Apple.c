#include "Apple.h"

// layer used for drawing and position information
Layer appleLayer = {
  (AbShape *)&circle2,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_RED,
  0
};

// apple struct
Apple a = {
  &appleLayer
};

// external reference to apple struct
Apple *apple = &a;

// draws the apple at its current position
void appleDraw() {
  int row, col;
  Region bounds;
  
  layerGetBounds(apple->appleLayer, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
  for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
      lcd_writeColor(apple->appleLayer->color);
    }
  }
}

// moves the apple to the passed position & redraws
// param newPos :: a pointer to an X,Y vector of the new position
void appleRespawn(Vec2* newPos) {
  apple->appleLayer->pos.axes[0] = newPos->axes[0];
  apple->appleLayer->pos.axes[1] = newPos->axes[1];
  appleDraw();
}
