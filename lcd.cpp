// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
  DDRA |= (1<<DDA0) |(1<<DDA1) |(1<<DDA2) |(1<<DDA3); //pins 22-25
  DDRB |= (1<<DDB4) |(1<<DDB6);                       //pins 10 = enable,12 = RS pin
}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    PORTA = data;           // set msb to the [7:4]address of "data"
    PORTB &= ~ (1<<PORTB6); // RS low
    
    PORTB |= (1<<PORTB4);   // Enable high
    delayUs(1);
    PORTB &= ~(1<<PORTB4);  // Enable low
    delayUs(delay);         //passing the specific delay for the required LCD procedure, since alot of them are different
    // celebrate
}

/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
    
    PORTA = command>>4;       // set msb [7:4] of command
    PORTB &= ~ (1<<PORTB6);   // RS low
    
    PORTB |= (1<<PORTB4);     // Enable high
    delayUs(1);
    PORTB &= ~(1<<PORTB4);    // Enable low
    PORTA  = command;         // set lsb [3:0] of command
    PORTB |= (1<<PORTB4);     // Enable high
    delayUs(1);             
    PORTB &= ~(1<<PORTB4);    // Enable low
    delayUs(delay);
    //party
}

/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
    PORTA = character >> 4; //assert [7:4] of command and set it to the PORT
    PORTB |= (1<<PORTB6);   // RS high
    PORTB |= (1<<PORTB4);   // Enable high
    delayUs(1);
    PORTB &= ~(1<<PORTB4);  // Enable low
    PORTA  =  character;    // assert [3:0] of command and set it to the PORT
    PORTB |= (1<<PORTB4);   // Enable high
    delayUs(1);             
    PORTB &= ~(1<<PORTB4);  // Enable low
    delayUs(46);
    //dance it out
}

/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
  while (*string != '\0'){  //increments the address of string and passes it through the write character function
    writeCharacter(*string);
    string++;
  }
//overthink this and be skeptical because this is literally as simple as it seems
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
  // x is the column, so 2 if statements will suffice with y as the parameters in the 8 bit command function
  
  if (x == 0){ //accidently had x=0, so the cursor wouldnt move
    eightBitCommandWithDelay(0b10000000 + y, 46); //the matrix of the 16x2 LCD had pin D7 enabled
                                          // so D[6:0] needs to be the address of each box
                                          //D7 is always one, and y increments which ever row you want hence x=0 pass 0b10000000 
                                          //into command line. I got lazy so I did it in hex 0x80
                                          
  }
	else if (x == 1)
  {
    eightBitCommandWithDelay(0b11000000 + y, 46);// same logic, when x=1 the addresses are 40-4F, which only involves the lower bits
                                          // being manipulated.
                                          //again, D7 needs to be one, but the msb [6:4] needs to equal 4 (100)
                                          //0b11000000 is D7 = 1, D[6:4] = 4 dec, and the rest is open to be shifted by y
  }
  
  
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  //clear display, just because it built upon itself when I tried uplaoding multiple times
  //eightBitCommandWithDelay(0b00000001, 5300);
  // Delay 15 milliseconds
  delayUs(20000);
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(0b0011, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(0b0011, 100);
  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay
  fourBitCommandWithDelay(0b0011, 100);
  // write 0b0010 to DB[7:4] and 100us delay.
  fourBitCommandWithDelay(0b0010, 100);
  // Function set in the command table with 53us delay
  //returns cursor to home position
  eightBitCommandWithDelay(0b00000010, 53);
  // Display off in the command table with 53us delay
  //turns display off, no need to set the cursor to blink or anything because it's off
  eightBitCommandWithDelay(0b00001000,53);
  // Clear display in the command table. Remember the delay is longer!!!
  //self explanitory clear display 8 bitcommand
  eightBitCommandWithDelay(0b00000001, 3000); //better safe than sorry for the delay
  // Entry Mode Set in the command table.
  //Entry mode set to increment, would be decrement if *string was string--
  eightBitCommandWithDelay(0b00000110, 53); // increment mode for the move cursor you want to add y, not decrement y
  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)

  eightBitCommandWithDelay(0b00001111,53); // sets cursor, display, and blink to "on"
} // pray it works

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}
