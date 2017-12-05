#include "Apple.h"

Layer appleLayer = {
  (AbShape *)&circle2,
  {(screenWidth/2)+10, (screenHeight/2)+5},
  {0,0}, {0,0},
  COLOR_RED,
  0
};

Apple a = {
  &appleLayer
};

Apple *apple = &a;

void appleRespawn(Vec2* newPos) {
  apple->appleLayer->pos = &newPos;
  appleDraw();
}

void appleDraw() {
  // since this is snake, we really only need to draw the head
  // and then clear the space left by the tail
  int row, col;
  Region bounds;
  
  // draw head
  layerGetBounds(apple->appleLayer, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
  for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
      lcd_writeColor(apple->appleLayer->color);
    }
  }
}
