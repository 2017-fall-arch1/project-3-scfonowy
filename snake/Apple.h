#ifndef APPLE__H_
#define APPLE__H_

#include <shape.h>
#include <stdbool.h>
#include <abCircle.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <stdlib.h>

typedef struct Apple {
	struct Layer* appleLayer;
} Apple;

Apple* appleInit();

void appleRespawn(Apple* apple);

bool appleOutOfBounds(Apple* apple, Region* bounds);

#endif
