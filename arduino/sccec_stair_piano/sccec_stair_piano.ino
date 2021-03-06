/*
 * Sierra College Computer Engineering Club
 * Staircase Piano Project
 * author: James Smith
 * 
 * This is the Arduino side of the stair project. The Arduino will use the buttonStates
 * library on 8 analog inputs to detect analog button presses (coming from laser pointers
 * pointed at photoresistors) and will keep track of which buttons have been pressed
 * by each setting a bit in a byte. The RPi will request this byte at a specific interval
 * over the I2C connection, and then the Arduino will reset the state and start tracking
 * more button presses.
 *
 * Requires an Arduino Mega, or any Arduino that has at least 10 analog inputs (8 for
 * button presses, 2 for I2C). This project could also be run on an Uno using digital
 * button presses.
 * 
 * TODO: Need to setup button code for all of the analog buttons, just using digital
 * buttons for now as placeholders
 */

#include <Wire.h>

// sets our address for the I2C connection
// The Arduino is setup to be a slave to the RPi
#define SLAVE_ADDRESS 0x04

const int buttonPin1 = 4;
const int buttonPin2 = 9;
/*
const int buttonPin3 = 5;
const int buttonPin4 = 6;
const int buttonPin5 = 7;
const int buttonPin6 = 8;
const int buttonPin7 = 10;
const int buttonPin8 = 11;
*/
const int analogPin1 = 2;

#include "buttonStates.h"
struct buttonState button1;
struct buttonState button2;
/*
struct buttonState button3;
struct buttonState button4;
struct buttonState button5;
struct buttonState button6;
struct buttonState button7;
struct buttonState button8;
*/
struct buttonState analog1;

// each bit in this byte will hold the key state for exactly one button, until that state data
// is transferred to the RPi over I2C
byte keyState = 0;

#define NOTE_1 0x01
#define NOTE_2 0x02
#define NOTE_3 0x04
#define NOTE_4 0x08
#define NOTE_5 0x10
#define NOTE_6 0x20
#define NOTE_7 0x40
#define NOTE_8 0x80

void setup()
{
  //Serial.begin(9600);
  pinMode( buttonPin1, INPUT );
  pinMode( buttonPin2, INPUT );
/*
  pinMode( buttonPin3, INPUT );
  pinMode( buttonPin4, INPUT );
  pinMode( buttonPin5, INPUT );
  pinMode( buttonPin6, INPUT );
  pinMode( buttonPin7, INPUT );
  pinMode( buttonPin8, INPUT );
*/
  pinMode( analogPin1, INPUT );
  
  // Setup I2C
  Wire.begin( SLAVE_ADDRESS );
  Wire.onRequest( sendData );
}
int lastState;
void loop()
{
  updateDigitalButton( &button1, buttonPin1 );
  updateDigitalButton( &button2, buttonPin2 );
/*
  updateDigitalButton( &button3, buttonPin1 );
  updateDigitalButton( &button4, buttonPin2 );
  updateDigitalButton( &button5, buttonPin1 );
  updateDigitalButton( &button6, buttonPin2 );
  updateDigitalButton( &button7, buttonPin1 );
  updateDigitalButton( &button8, buttonPin2 );
*/
  updateInvAnalogButton( &analog1, analogPin1, 700 );
  //Serial.println( analogRead( analogPin1 ) );
  
  if( analog1.Pressed || button1.Pressed )
  {
    keyState |= NOTE_1;
  }
  if( button2.Pressed )
  {
    keyState |= NOTE_2;
  }
/*
  if( button3.Pressed )
  {
    keyState |= NOTE_3;
  }
  if( button4.Pressed )
  {
    keyState |= NOTE_4;
  }
  if( button5.Pressed )
  {
    keyState |= NOTE_5;
  }
  if( button6.Pressed )
  {
    keyState |= NOTE_6;
  }
  if( button7.Pressed )
  {
    keyState |= NOTE_7;
  }
  if( button8.Pressed )
  {
    keyState |= NOTE_8;
  }
*/
}

// Send data to the RPi over the Wire protocol (I2C)
void sendData()
{
  Wire.write( keyState );
  keyState = 0;
}
