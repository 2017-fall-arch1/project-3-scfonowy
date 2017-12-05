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

char score = 0; // current score
char scoreString[] = "00"; // string used to display current score

// the field outline (i.e. fence)
AbRectOutline field = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 10, screenHeight/2 - 10}
};

// the region of the fence (used for bounds checking)
Region fieldFence;

// layer of the fence
Layer fieldLayer = {
  (AbShape *)&field,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_WHITE,
  0 // next layer will be the snake
};

// updates score label with current score
void scoreUpdate() {
  // NOTE: 48 is ASCII value of '0'
  scoreString[0] = 48 + (score/10);
  scoreString[1] = 48 + (score%10);
  
  // draw the string
  drawString5x7(screenWidth-20,0,scoreString, COLOR_WHITE, COLOR_BLACK);
}

// checks switch states & updates direction
void directionUpdate() {
  unsigned int switches = p2sw_read(); // get switch info
  char switchPositions = switches; // get current values
  char switchesChanged = switches >> 8; // get changed switches
  if (switchesChanged) { // a switch has changed
    speakerOn(); // make movement beep
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
  clearScreen(COLOR_BLACK); // clear screen
  
  snakeInit(); // reset snake
  
  // reset field bounds
  abShapeGetBounds((AbShape *)&field, &fieldLayer.pos, &fieldFence);
  
  // redraw the game
  layerInit(&fieldLayer);
  layerDraw(&fieldLayer);
  
  // reset & redraw score
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
  
  gameReset(); // initial game reset
  
  enableWDTInterrupts();
  or_sr(0x8);
  
//  for(;;) {
//    while(!redrawScreen) {
//      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
//      or_sr(0x10);
//    }
//    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
//    redrawScreen = 0;
//    snakeDraw(); // only need to redraw snake
//  }
}

void wdt_c_handler() {
  static short count = 0;
  P1OUT |= GREEN_LED; // green LED on when CPU on
  
  if (count % 10 == 0) {
    // check switches more frequently (better UX)
    directionUpdate();
  }
  
  if (count == 100) {
    // turn off speaker
    speakerOff();
    
    // game over state
    if (snakeIsOutOfBounds(&fieldFence) || snakeIsEatingSelf()) {
      speakerOn(); // play the Bad Beep
      speakerSetTone(6000);
      gameReset(); // reset game
    } else {
      // snake is eating apple, play noise & grow snake
      if (snakeIsEatingApple()) {
        speakerOn(); // play the Good Beep
        speakerSetTone(1500);
        
        score++; // update score
        scoreUpdate();
        
        snakeGrow(); // grow snake
        snakeUpdate();
        
        // respawn apple at tail position
        appleRespawn(&snake->segments[snake->size]);
      } else {
        // game continues, update snake
        snakeUpdate();
      } 
    }
    snakeDraw(); // draw snake
    count = 0; // reset count
  }
  
  count++;
  P1OUT &= ~GREEN_LED; // green LED off when CPU off
}
