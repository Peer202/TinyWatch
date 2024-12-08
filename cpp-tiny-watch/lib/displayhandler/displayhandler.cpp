//#include <Arduino.h>
#include <avr/io.h>
#include "displayhandler.h"

//#define LED_PINS PIN_PA2,PIN_PA1,PIN_PC3,PIN_PC2,PIN_PC1,PIN_PC0,PIN_PB1,PIN_PB4,PIN_PB5,PIN_PA7,PIN_PA6,PIN_PA5
#define HOUR_GND_PIN PIN_PA4 
#define MIN_GND_PIN PIN_PA3
#define LED_PINS {{1,2},{1,1},{3,3},{3,2},{3,1},{3,0},{2,1},{2,4},{2,5},{1,7},{1,6},{1,5}}
//#define HOUR_GND_PIN {1,4}
//#define MIN_GND_PIN 

#define Blink_duration 10
#define Diplay_duration 4000

void TogglePin(int Port,int PinNumber)
{ 
  int Pinmap[] = {PIN0_bm,PIN1_bm,PIN2_bm,PIN3_bm,PIN4_bm,PIN5_bm,PIN6_bm,PIN7_bm};

  switch (Port)
  {
  case 1:
    PORTA.OUTTGL = Pinmap[PinNumber];
    break;
  case 2:
    PORTB.OUTTGL = Pinmap[PinNumber];
    break;
  case 3:
    PORTC.OUTTGL = Pinmap[PinNumber];
    break;
  default:
    break;
  }
}

void TogglePinDir(int Port,int PinNumber)
{ 
  int Pinmap[] = {PIN0_bm,PIN1_bm,PIN2_bm,PIN3_bm,PIN4_bm,PIN5_bm,PIN6_bm,PIN7_bm};
  switch (Port)
  {
  case 1:
    PORTA.DIRSET = Pinmap[PinNumber];
    break;
  case 2:
    PORTB.DIRSET = Pinmap[PinNumber];
    break;
  case 3:
    PORTC.DIRSET = Pinmap[PinNumber];
    break;
  default:
    break;
  }
}

int initdisplay()
{
    // Set Pins as output
    int pinsLed [][2] = LED_PINS;
    for (int i = 0; i < 12; i++)
    {
      TogglePinDir(pinsLed[i][0],pinsLed[i][1]);
    }

    TogglePinDir(1,4); // Hour Pin
    TogglePinDir(1,3); // Minute Pin
}



/*
This function handles the LEDs to display a supplied time

Input: 
    int hour: The hour in format [0-24]
    int minute: the minute in format [0-60]
    
Output: 1 if successfull
*/

int displayTime(int hour, int minute)
{
  
  pin_size_t pinsLed [] = {LED_PINS};
  int iLedHour; int iLedMin;
    
  // calculate which LED to turn on

  if(hour > 12)
  {
    iLedHour = hour - 12;
  }
  else 
  {
    iLedHour = hour;
  }
   

  iLedMin = (int) round((double) minute / 5);
  // Enable LED
  for (size_t i = 0; i < (int) (Diplay_duration / Blink_duration); i++)
  {
    digitalWrite(HOUR_GND_PIN,HIGH);
    digitalWrite(MIN_GND_PIN,LOW);
    digitalWrite(pinsLed[iLedHour],HIGH);
    digitalWrite(pinsLed[iLedMin],LOW);
    

    delay(Blink_duration);

    digitalWrite(HOUR_GND_PIN,LOW);
    digitalWrite(MIN_GND_PIN,HIGH);
    digitalWrite(pinsLed[iLedHour],LOW);
    digitalWrite(pinsLed[iLedMin],HIGH);
    
    delay(Blink_duration);

  }
  digitalWrite(pinsLed[iLedHour],LOW);
  digitalWrite(pinsLed[iLedMin],LOW);
  digitalWrite(HOUR_GND_PIN,LOW);
  digitalWrite(MIN_GND_PIN,LOW);
  
  return 1;
}

int debugBlink ()
{
  digitalWrite(PIN_PA1,HIGH);
  digitalWrite(HOUR_GND_PIN,HIGH);
  delay(250);
  digitalWrite(PIN_PA1,LOW);
  digitalWrite(HOUR_GND_PIN,LOW);
}