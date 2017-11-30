#ifndef APPLE__H_
#define APPLE__H_

#include <shape.h>
#include <stdbool.h>
#include <abCircle.h>

typedef struct Apple {
	struct Layer* appleLayer;
} Apple;

Apple* appleInit();

void appleRespawn(Apple* apple);

bool appleOutOfBounds(Apple* apple, Region* bounds);

#endif