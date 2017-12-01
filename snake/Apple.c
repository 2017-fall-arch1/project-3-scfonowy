#include "Apple.h"

Apple* appleInit() {
  Apple* apple = (Apple *)malloc(sizeof(Apple)); // create apple
  
  Layer* appleLayer = (Layer *)malloc(sizeof(Layer)); // create and configure apple layer
  appleLayer->abShape = (AbShape *)&circle2;
  appleLayer->posLast = (Vec2){0,0};
  appleLayer->pos = (Vec2){screenWidth/2 + 10, screenHeight/2 + 10};
  appleLayer->posNext = (Vec2){0,0};
  appleLayer->color = COLOR_RED;
  appleLayer->next = 0;
  
  return apple;
}

void appleRespawn(Apple* apple);

bool appleOutOfBounds(Apple* apple, Region* bounds);
