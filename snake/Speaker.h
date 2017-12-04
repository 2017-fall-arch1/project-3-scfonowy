#ifndef SPEAKER_H_
#define SPEAKER_H_

// initializes the speaker for output
void speakerInit(void);

// turns the speaker on
void speakerOn();

// turns the speaker off
void speakerOff();

// sets the tone the speaker should play
// param tone :: the frequency of the tone
void speakerSetTone(short tone);

#endif // included
