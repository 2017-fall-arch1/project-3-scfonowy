#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "Snake.h"
#include "Apple.h"

Snake* snake = snakeInit();
Apple* apple = appleInit();
AbRectOutline field = {
	abRectOutlineGetBounds, abRectOutlineCheck,
	{screenWidth/2 - 10, screenHeight/2 - 10}
}

Layer fieldLayer = {
	(AbShape *)&field,
	{screenWidth/2, screenHeight/2},
	{0,0},{0,0},
	COLOR_BLACK,
	snake->headLayer // next layer is the snake
}

void main() {
    // setup
    configureClocks();
    lcd_init();
    clearScreen(COLOR_BLUE);
	
	layerInit(&fieldLayer);
	layerDraw(&fieldLayer);
	
	enableWDTInterrupts();
	or_sr(0x8);
	
	for(;;) {
		while(!redrawScreen) {
			or_sr(0x10);
		}
		redrawScreen = 0;
		snakeDraw(snake);
	}
}

void wdt_c_handler() {
	static short count = 0;
	if (count == 15) {
		snakeUpdate(snake);
		count = 0;
	}
	count++;
}
