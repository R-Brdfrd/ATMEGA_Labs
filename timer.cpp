// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
  //CTC Mode
    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B |= (1<<WGM12);
    TCCR1B &= ~(1<<WGM13);

    

    TCCR1B |= (1<<CS11); //set to 8 with OCR1A val of 2 since we need 16 counts
    TCCR1B &= ~(1<<CS12);
    TCCR1B &= ~(1<<CS10);
    OCR1A = 2;
    //OCR1AH = 2;
    //OCR1AL = 0;

}


/* This delays the program an amount specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    //1us times delay aomount
    unsigned int delayCnt = 0;
    TCNT1 = 0; //starting the timer at 0 instead of some random junk number
    TIFR1 |= (1 << OCF1A); // set compare flag to start timer
    
    while (delayCnt < delay) {
        if (TIFR1 & (1 << OCF1A)) {
            //increment only while the flag is set (timer reached maxval= OCR0A)
            delayCnt++;
            TIFR1 |= (1 << OCF1A);
        }
    }
}


