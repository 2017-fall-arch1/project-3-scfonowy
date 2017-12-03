#include "Apple.h"

Layer appleLayer = {
    (AbShape *)&circle2,
    {(screenWidth/2)+10, (screenHeight/2)+5},
    {0,0}, {0,0},
    COLOR_RED,
    0
}

Apple apple = {
    &appleLayer;
};

void appleRespawn();

bool appleOutOfBounds();
