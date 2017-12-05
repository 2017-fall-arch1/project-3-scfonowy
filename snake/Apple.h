#ifndef APPLE__H_
#define APPLE__H_

#include <shape.h>
#include <stdbool.h>
#include <abCircle.h>
#include <lcdutils.h>
#include <lcddraw.h>

typedef struct Apple {
    Layer* appleLayer;
} Apple;

// reference to main apple object
extern Apple *apple;

// moves the apple to the passed position & redraws
// param newPos :: a pointer to an X,Y vector of the new position
void appleRespawn(Vec2* newPos);

#endif
