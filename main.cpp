// Author:         
// Net ID:         
// Date:          
// Assignment:     Lab 2
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"



/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum purdy {frog, hold_up, bicycle, wait_a_sec} stateType;
volatile stateType state = frog;

//volatile stateType state = frog;
int i=0;


int main(){
  
//initialize timer, LEDs, Switch, and interrupts
  initTimer0();
  initLED();
  initSwitchPB3();
  sei();

  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */
	while (1) {

        if (i<16){
        turnOnLEDWithChar(i);
        i++;
        }
        else
        i=0;

    switch (state) {
      case frog:
        delayMs(100);
      break;

      case hold_up:
        delayMs(100);
        state = bicycle;
      break;

      case bicycle: 
        delayMs(200);
      break;

      case wait_a_sec: 
        delayMs(100);
        state = frog;
      break;
    
    /*default:
      break;*/
    }


	}
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
ISR(PCINT0_vect){
  if (state == frog){
    state = hold_up;
  }
  else if (state == bicycle){
    state = wait_a_sec;
  }
  //else
  //  state = hold_up;
  
}