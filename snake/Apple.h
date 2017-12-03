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

extern Apple *apple;

void appleRespawn();

#endif
