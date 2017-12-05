#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "Snake.h"
#include "Apple.h"
#include "Speaker.h"

#define GREEN_LED BIT6  // Green LED bit

#define SW1 BIT0        // Bits for different switches on P2
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)    // All the switches on P2

u_int bgColor = COLOR_BLACK;
char redrawScreen = 1;
char score = 0;
char scoreString[] = "00";

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

// updates score label with current score
void scoreUpdate() {
  scoreString[0] = 48 + (score/10);
  scoreString[1] = 48 + (score%10);
  drawString5x7(screenWidth-20,0,scoreString, COLOR_WHITE, COLOR_BLACK);
}

// checks switch states & updates direction
void directionUpdate() {
  unsigned int switches = p2sw_read();
  char switchPositions = switches;
  char switchesChanged = switches >> 8;
  if (switchesChanged) { // a switch has changed
    speakerOn();
    speakerSetTone(4000);
    if (!(switchPositions & SW1)) { // left
      snakeChangeDirection(SW1);
    } else if (!(switchPositions & SW2)) { // up
      snakeChangeDirection(SW2);
    } else if (!(switchPositions & SW3)) { // down
      snakeChangeDirection(SW3);
    } else if (!(switchPositions & SW4)) { // right
      snakeChangeDirection(SW4);
    }
  }
}

// resets and redraws the game
void gameReset() {
  clearScreen(COLOR_BLACK);
  
  snakeInit();
  
  abShapeGetBounds((AbShape *)&field, &fieldLayer.pos, &fieldFence);
  
  layerInit(&fieldLayer);
  layerDraw(&fieldLayer);
  
  drawString5x7(10,0,"Score:", COLOR_WHITE, COLOR_BLACK);
  score = 0;
  scoreString[0] = '0';
  scoreString[1] = '0';
  scoreUpdate();
}

void main() {
  P1DIR |= GREEN_LED;    /**< Green led on when CPU on */
  P1OUT |= GREEN_LED;
  
  // setup
  fieldLayer.next = apple->appleLayer;
  apple->appleLayer->next = snake->headLayer;
  
  configureClocks();
  lcd_init();
  speakerInit();
  p2sw_init(15);
  
  gameReset();
  
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
  if (count % 10 == 0) {
    directionUpdate();
  }
  if (count == 100) {
    speakerOff();
    if (snakeIsOutOfBounds(&fieldFence)) {
      speakerOn();
      speakerSetTone(6000);
      gameReset();
    } else {
      if (snakeIsEatingApple()) {
        speakerOn();
        speakerSetTone(1500);
        score++;
        scoreUpdate();
        snakeGrow();
        snakeUpdate();
        appleRespawn(&snake->segments[snake->size]);
      } else {
        snakeUpdate();
        scoreUpdate();
      } 
    }
    snakeDraw();
    count = 0;
  }
  count++;
  P1OUT &= ~GREEN_LED;
}
