#include <msp430.h>
#include "libTimer.h"
#include "Speaker.h"

// sets up the speaker for output
void speakerInit() {
    /* Direct timer A output "TA0.1" to P2.6.  
     According to table 21 from data sheet:
     P2SEL2.6, P2SEL2.7, and P2SEL.7 must be zero
     P2SEL.6 must be 1
     Also: P2.6 direction must be output */
    timerAUpmode();		// used to drive speaker
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    speakerOn();		// enable output to speaker (P2.6)
}

// turns speaker on by enabling output on P2DIR bit 6
void speakerOn() {
    P2DIR |= BIT6;
}

// turns speaker off by disabling output on P2DIR bit 6
void speakerOff() {
    P2DIR &= ~BIT6;
}

// sets the "tone" of the speaker to the passed short
// param tone :: the tone to set
void speakerSetTone(short tone) {
    CCR0 = tone;
    CCR1 = tone >> 1;
}
