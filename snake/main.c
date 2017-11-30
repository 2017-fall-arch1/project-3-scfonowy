#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "Snake.h"
#include "Apple.h"


int main() {
	// setup
    configureClocks();
    lcd_init();
	clearScreen(COLOR_BLUE);
	
	return 0;
}