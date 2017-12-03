#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "Snake.h"
#include "Apple.h"

#define GREEN_LED BIT6;

u_int bgColor = COLOR_BLACK;
char redrawScreen = 1;
char score = 0;

AbRectOutline field = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 10, screenHeight/2 - 10}
};

Region fieldFence;

Layer fieldLayer = {
  (AbShape *)&field,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_WHITE,
  0 // next layer will be the snake
};

void main() {
  P1DIR |= GREEN_LED;    /**< Green led on when CPU on */
  P1OUT |= GREEN_LED;
  
  // setup
  fieldLayer.next = apple->appleLayer;
  apple->appleLayer->next = snake->headLayer;
  
  configureClocks();
  lcd_init();
  p2sw_init(15);
  clearScreen(COLOR_BLACK);
  
  snakeInit();
  
  //appleInit();
  layerGetBounds(&fieldLayer, &fieldFence);
  
  layerInit(&fieldLayer);
  layerDraw(&fieldLayer);
  
  drawString5x7(10,0,"Score:", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(screenWidth-20,0,"00", COLOR_WHITE, COLOR_BLACK);
  
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
    snakeDraw();
  }
}

void wdt_c_handler() {
  static short count = 0;
  P1OUT |= GREEN_LED;
  if (count == 50) {
    if (!snakeIsOutOfBounds(&fieldFence)) {
      snakeUpdate();
    }
    snakeDraw();
    score++;
    count = 0;
  }
  count++;
  P1OUT &= ~GREEN_LED;
}
