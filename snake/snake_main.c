#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "Snake.h"
#include "Apple.h"

#define GREEN_LED BIT6;

u_int bgColor = COLOR_BLUE;
int redrawScreen = 1;

Snake* snake;
Apple* apple;

AbRectOutline field = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 10, screenHeight/2 - 10}
};

Layer fieldLayer = {
  (AbShape *)&field,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_BLACK,
  0 // next layer is the snake
};

void main() {
  P1DIR |= GREEN_LED;    /**< Green led on when CPU on */
  P1OUT |= GREEN_LED;
  
  // setup
  snake = snakeInit();
  snakeGrow(snake);
  snakeGrow(snake);
  apple = appleInit();
  fieldLayer.next = apple->appleLayer;
  apple->appleLayer->next = snake->headLayer;
  
  configureClocks();
  lcd_init();
  p2sw_init(1);
  clearScreen(COLOR_BLUE);
  
  layerInit(&fieldLayer);
  layerDraw(&fieldLayer);
  
  enableWDTInterrupts();
  or_sr(0x8);
  
  for(;;) {
    while(!redrawScreen) {
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    //layerDraw(&fieldLayer);
    snakeDraw(snake);
  }
}

void wdt_c_handler() {
  static short count = 0;
  P1OUT |= GREEN_LED;
  if (count == 50) {
    if (p2sw_read()) {
      redrawScreen = 1;
    }
    snakeUpdate(snake);
    count = 0;
  }
  count++;
  P1OUT &= ~GREEN_LED;  
}
