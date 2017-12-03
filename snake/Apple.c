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

void appleRespawn();

bool appleOutOfBounds();
