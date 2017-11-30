#include "Apple.h"

Apple* appleInit() {
	(Apple *)apple = malloc(sizeof(Apple)); // create apple
	
	(Layer *)appleLayer = malloc(sizeof(Layer)); // create and configure apple layer
	appleLayer->abShape = &circle2;
	appleLayer->posLast = {0,0};
	appleLayer->pos = {screenWidth/2 + 10, screenHeight/2 + 10};
	appleLayer->posNext = {0,0};
	appleLayer->color = COLOR_RED;
	appleLayer->next = 0;
	
	return apple;
}

void appleRespawn(Apple* apple);

bool appleOutOfBounds(Apple* apple, Region* bounds);